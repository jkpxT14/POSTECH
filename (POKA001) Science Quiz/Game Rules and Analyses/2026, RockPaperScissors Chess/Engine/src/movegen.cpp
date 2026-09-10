#include "movegen.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <unordered_set>

namespace rpsc {
struct MoveGeneratorAccess {
    static MoveOutcome apply(Position& p, const Move& m, Orientation o, UndoState& u) {
#ifndef NDEBUG
        Orientation checked;
        assert(p.validate_path(m, &checked) && checked == o);
#endif
        return p.apply_legal_move(m, o, u);
    }
};
namespace {
constexpr std::array<Direction,4> Dirs{Direction::North,Direction::South,Direction::East,Direction::West};
constexpr std::array<Direction,4> Opp{Direction::South,Direction::North,Direction::West,Direction::East};
constexpr std::array<Item,6> Rotations{Item::RotateNorth,Item::RotateSouth,Item::RotateEast,Item::RotateWest,Item::RotateLeft,Item::RotateRight};

struct StepTable {
    std::array<std::array<Square,4>,64> next{};
    std::array<std::uint64_t,64> neighbors{};
    StepTable() {
        for (int s=0;s<64;++s) {
            for (int di=0;di<4;++di) {
                int f=file_of(Square(s)),r=rank_of(Square(s));
                switch(Dirs[di]){case Direction::North:++r;break;case Direction::South:--r;break;case Direction::East:++f;break;case Direction::West:--f;break;}
                next[s][di]=valid_square(f,r)?make_square(f,r):NoSquare;
                if(next[s][di]!=NoSquare) neighbors[s]|=1ULL<<unsigned(next[s][di]);
            }
        }
    }
};
const StepTable& steps(){static const StepTable t;return t;}
int pc64(std::uint64_t x){
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int n=0;while(x){x&=x-1;++n;}return n;
#endif
}

Orientation modifier_orientation(Orientation o,Item item){return OrientationTable::instance().apply_rotation(o,item);}
int modifier_distance(Orientation original,Item item){const auto&t=OrientationTable::instance();Orientation o=modifier_orientation(original,item);int n=base_roll_length(t.top_gesture(o));if(item==Item::StepShort)--n;if(item==Item::StepLong)++n;return n;}

void exhaustive_paths(const Position& p, PieceId id, Move& m, Square cur, int rem,
                      bool has_last, Direction last, std::uint64_t occupied,
                      std::uint64_t enemies, std::vector<Move>& out) {
    if(rem==0){out.push_back(m);return;}
    const bool final=rem==1;
    for(int di=0;di<4;++di){
        Direction d=Dirs[di]; if(has_last&&d==Opp[int(last)])continue;
        Square next=steps().next[int(cur)][di];
        if(next==NoSquare||(occupied&(1ULL<<unsigned(next))))continue;
        if(final&&pc64(steps().neighbors[int(next)]&enemies)>=2)continue;
        m.path[m.path_length++]=next;
        exhaustive_paths(p,id,m,next,rem-1,true,d,occupied,enemies,out);
        --m.path_length;
    }
}
void exhaustive_item(const Position& p, PieceId id, const PieceState& pc, Item item, Square push,
                     std::uint64_t occupied, std::uint64_t enemies, std::vector<Move>& out){
    Move m;m.piece=id;m.item=item;m.push_to=push;m.path[0]=pc.square;m.path_length=1;
    int distance=modifier_distance(pc.orientation,item);Square start=pc.square;
    if(item==Item::Push)start=push;
    exhaustive_paths(p,id,m,start,distance,false,Direction::North,occupied,enemies,out);
}

std::uint32_t partial_index(Square s,Orientation o,int rem,bool has_last,Direction last){auto st=OrientationTable::instance().gesture_state_id(o);auto lc=has_last?unsigned(last):4u;return unsigned(s)|(unsigned(st)<<6)|(unsigned(rem)<<11)|(lc<<15);}
struct Scratch {
    static constexpr std::size_t FinalSize=1u<<14,Mask=FinalSize-1;
    std::array<std::uint32_t,1u<<18> partial{};std::uint32_t partial_generation=1;
    std::array<Key,FinalSize> keys{};std::array<std::uint32_t,FinalSize> marks{};std::uint32_t final_generation=1;
    std::uint32_t next_partial(){if(++partial_generation==0){partial.fill(0);partial_generation=1;}return partial_generation;}
    std::uint32_t next_final(){if(++final_generation==0){marks.fill(0);final_generation=1;}return final_generation;}
    bool insert(Key k,std::uint32_t g){std::size_t slot=std::size_t((k*11400714819323198485ULL)>>50)&Mask;for(std::size_t probe=0;probe<FinalSize;++probe){if(marks[slot]!=g){marks[slot]=g;keys[slot]=k;return true;}if(keys[slot]==k)return false;slot=(slot+1)&Mask;}return true;}
};
Scratch& scratch(){static thread_local Scratch v;return v;}
struct Gen {
    Position& p;
    Color mover, opponent;
    bool tactical;
    Scratch& scratch;
    std::uint32_t partial_generation=0, final_generation=0;
    std::vector<SearchMove> out;
    std::array<std::int8_t,64> enemy_at{};
    int own_alive=0, opponent_alive=0;
};

int combat(Gesture a, Gesture b) {
    if (a == b) return 0;
    if ((a == Gesture::Scissors && b == Gesture::Paper) ||
        (a == Gesture::Rock && b == Gesture::Scissors) ||
        (a == Gesture::Paper && b == Gesture::Rock)) return 1;
    return -1;
}

// All endpoints emitted during one generation share the same parent position.
// Therefore successor equality can be decided from just the fields that change,
// instead of make -> search_key -> undo for every path endpoint.  This signature
// is collision-free (bit-packed, not hashed) and mirrors Position::search_key's
// reduced orientation semantics exactly.
Key successor_signature(const Gen& g, const Move& m, Square cur, Orientation o,
                        std::uint64_t enemies, int& swing, PieceId& captured,
                        bool& has_capture, bool& reset) {
    const auto& t = OrientationTable::instance();
    std::uint64_t near = steps().neighbors[int(cur)] & enemies;
    int outcome = 0; // quiet/tie
    int enemy_index = 0;
    swing = 0;
    captured = PieceId::W1; has_capture = false; reset = false;
    if (near) {
        Square es;
#if defined(__GNUC__) || defined(__clang__)
        es = Square(__builtin_ctzll(near));
#else
        es = Square(0); while (((near >> unsigned(es)) & 1ULL) == 0) es = Square(int(es)+1);
#endif
        const int ei = g.enemy_at[unsigned(es)];
        if (ei >= 0) {
            enemy_index = ei;
            const int result = combat(t.top_gesture(o), t.top_gesture(g.p.piece(PieceId(ei)).orientation));
            if (result > 0) {
                swing = 1; captured = PieceId(ei); has_capture = true; reset = g.opponent_alive == 1;
                outcome = reset ? 3 : 1; // reset-win / win
            } else if (result < 0) {
                swing = -1; captured = m.piece; has_capture = true; reset = g.own_alive == 1;
                outcome = reset ? 4 : 2; // reset-loss / loss
            }
        }
    }

    const unsigned bucket = m.item == Item::None ? 0u : unsigned(item_bucket(m.item) + 1); // 0..3
    Key sig = Key(outcome) | (Key(bucket) << 3) | (Key(piece_index(m.piece)) << 5);
    // A losing mover is removed, so its endpoint/orientation are not part of the
    // resulting search state.  After Reset all pieces are canonical again.
    if (outcome == 0 || outcome == 1) {
        sig |= Key(unsigned(cur)) << 8;
        sig |= Key(t.gesture_state_id(o)) << 14;
    }
    if (outcome == 1) sig |= Key(unsigned(enemy_index)) << 19;
    return sig;
}

void emit(Gen& g, Move& m, Square cur, Orientation o, std::uint64_t enemies) {
    int swing = 0; PieceId captured = PieceId::W1; bool has_capture = false, reset = false;
    const Key signature = successor_signature(g, m, cur, o, enemies, swing, captured, has_capture, reset);
    if (g.tactical && swing == 0) return;
    if (g.scratch.insert(signature, g.final_generation)) g.out.push_back({m,swing,o,captured,has_capture,reset});
}
void reduced_paths(Gen&g,PieceId id,Move&m,Square cur,Orientation o,int rem,bool has_last,Direction last,
                   std::uint64_t occupied,std::uint64_t enemies){
    if(rem==0){emit(g,m,cur,o,enemies);return;}
    bool final=rem==1;const auto&t=OrientationTable::instance();
    for(int di=0;di<4;++di){Direction d=Dirs[di];if(has_last&&d==Opp[int(last)])continue;Square next=steps().next[int(cur)][di];if(next==NoSquare||(occupied&(1ULL<<unsigned(next))))continue;if(final&&pc64(steps().neighbors[int(next)]&enemies)>=2)continue;Orientation no=t.roll(o,d);auto idx=partial_index(next,no,rem-1,true,d);if(g.scratch.partial[idx]==g.partial_generation)continue;g.scratch.partial[idx]=g.partial_generation;m.path[m.path_length++]=next;reduced_paths(g,id,m,next,no,rem-1,true,d,occupied,enemies);--m.path_length;}
}
void reduced_item(Gen&g,PieceId id,const PieceState&pc,Item item,Square push,std::uint64_t occupied,std::uint64_t enemies){
    Move m;m.piece=id;m.item=item;m.push_to=push;m.path[0]=pc.square;m.path_length=1;Orientation o=modifier_orientation(pc.orientation,item);int distance=modifier_distance(pc.orientation,item);Square start=pc.square;if(item==Item::Push)start=push;g.partial_generation=g.scratch.next_partial();g.scratch.partial[partial_index(start,o,distance,false,Direction::North)]=g.partial_generation;reduced_paths(g,id,m,start,o,distance,false,Direction::North,occupied,enemies);
}
std::vector<SearchMove> generate_search_internal(Position&p,bool tactical){
    Color mover=p.side_to_move(); Color opponent=opposite(mover);
    std::uint64_t occupancy=0,enemies=0;
    int own_alive=0,opponent_alive=0;
    std::array<std::int8_t,64> enemy_at{}; enemy_at.fill(-1);
    for(int i=0;i<PieceCount;++i){
        auto id=PieceId(i); const auto&pc=p.piece(id); if(!pc.alive())continue;
        const auto bit=1ULL<<unsigned(pc.square); occupancy|=bit;
        if(piece_color(id)==opponent){enemies|=bit;enemy_at[unsigned(pc.square)]=std::int8_t(i);++opponent_alive;}
        else ++own_alive;
    }
    Gen g{p,mover,opponent,tactical,scratch(),0,0,{},enemy_at,own_alive,opponent_alive};
    g.final_generation=g.scratch.next_final(); g.out.reserve(768);
    for(int i=0;i<PieceCount;++i){
        auto id=PieceId(i);const auto&pc=p.piece(id);if(!pc.alive()||piece_color(id)!=mover)continue;
        std::uint64_t occupied=occupancy&~(1ULL<<unsigned(pc.square));
        reduced_item(g,id,pc,Item::None,NoSquare,occupied,enemies);
        if(p.item_count(mover,0)>0)for(int di=0;di<4;++di){Square push=steps().next[int(pc.square)][di];if(push!=NoSquare&&!(occupied&(1ULL<<unsigned(push))))reduced_item(g,id,pc,Item::Push,push,occupied,enemies);}
        if(p.item_count(mover,1)>0){std::array<bool,27>seen{};const auto&t=OrientationTable::instance();for(Item rot:Rotations){Orientation ro=t.apply_rotation(pc.orientation,rot);auto st=t.gesture_state_id(ro);if(seen[st])continue;seen[st]=true;reduced_item(g,id,pc,rot,NoSquare,occupied,enemies);}}
        if(p.item_count(mover,2)>0){reduced_item(g,id,pc,Item::StepShort,NoSquare,occupied,enemies);reduced_item(g,id,pc,Item::StepLong,NoSquare,occupied,enemies);}
    }
    return g.out;
}
} // namespace

std::vector<Move> generate_legal_moves(const Position&p){
    std::vector<Move>out;out.reserve(4096);Color mover=p.side_to_move();
    std::uint64_t occupancy=0,enemies=0;
    for(int i=0;i<PieceCount;++i){auto id=PieceId(i);const auto&pc=p.piece(id);if(!pc.alive())continue;auto bit=1ULL<<unsigned(pc.square);occupancy|=bit;if(piece_color(id)!=mover)enemies|=bit;}
    for(int i=0;i<PieceCount;++i){auto id=PieceId(i);const auto&pc=p.piece(id);if(!pc.alive()||piece_color(id)!=mover)continue;std::uint64_t occupied=occupancy&~(1ULL<<unsigned(pc.square));exhaustive_item(p,id,pc,Item::None,NoSquare,occupied,enemies,out);if(p.item_count(mover,0)>0)for(int di=0;di<4;++di){Square push=steps().next[int(pc.square)][di];if(push!=NoSquare&&!(occupied&(1ULL<<unsigned(push))))exhaustive_item(p,id,pc,Item::Push,push,occupied,enemies,out);}if(p.item_count(mover,1)>0)for(Item r:Rotations)exhaustive_item(p,id,pc,r,NoSquare,occupied,enemies,out);if(p.item_count(mover,2)>0){exhaustive_item(p,id,pc,Item::StepShort,NoSquare,occupied,enemies,out);exhaustive_item(p,id,pc,Item::StepLong,NoSquare,occupied,enemies,out);}}
    return out;
}
std::vector<Move> generate_unique_moves(Position&p){auto all=generate_legal_moves(p);std::vector<Move>u;u.reserve(all.size());std::unordered_set<Key>seen;seen.reserve(all.size()*2);for(const auto&m:all){UndoState st;p.do_move(m,st);Key k=p.key();p.undo_move(st);if(seen.insert(k).second)u.push_back(m);}return u;}
std::vector<SearchMove> generate_search_moves_info(Position&p){return generate_search_internal(p,false);}
std::vector<SearchMove> generate_tactical_moves_info(Position&p){return generate_search_internal(p,true);}
std::vector<Move> generate_search_moves(Position&p){auto info=generate_search_moves_info(p);std::vector<Move>m;m.reserve(info.size());for(const auto&e:info)m.push_back(e.move);return m;}
} // namespace rpsc

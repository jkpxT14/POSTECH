#include "evaluate.h"
#include <array>
#include <utility>
namespace rpsc { namespace {
constexpr Value ItemReserveValue = 6;
constexpr std::array<std::pair<int,int>,4> Directions{{{0,1},{0,-1},{1,0},{-1,0}}};
int first_step_mobility(const Position& position, PieceId id){const auto& piece=position.piece(id); if(!piece.alive()) return 0; int mobility=0; const int file=file_of(piece.square),rank=rank_of(piece.square); for(const auto& [df,dr]:Directions){if(!valid_square(file+df,rank+dr)) continue; if(!position.occupied(make_square(file+df,rank+dr),id)) ++mobility;} return mobility;}
int second_step_continuations(const Position& position, PieceId id){const auto& piece=position.piece(id); if(!piece.alive()) return 0; int c=0; const int file=file_of(piece.square),rank=rank_of(piece.square); for(const auto& [df,dr]:Directions){int ff=file+df,fr=rank+dr; if(!valid_square(ff,fr)||position.occupied(make_square(ff,fr),id)) continue; for(const auto& [ndf,ndr]:Directions){if(ndf==-df&&ndr==-dr) continue; int sf=ff+ndf,sr=fr+ndr; if(!valid_square(sf,sr)) continue; if(!position.occupied(make_square(sf,sr),id)) ++c;}} return c;}
}
Value evaluate_white(const Position& position){Value value=CaptureValue*(position.captures(Color::White)-position.captures(Color::Black)); value+=12*(position.alive_count(Color::White)-position.alive_count(Color::Black)); int wi=0,bi=0; for(int b=0;b<3;++b){wi+=position.item_count(Color::White,b);bi+=position.item_count(Color::Black,b);} value+=ItemReserveValue*(wi-bi); for(int i=0;i<PieceCount;++i){auto id=static_cast<PieceId>(i); if(!position.piece(id).alive()) continue; int mobility=2*first_step_mobility(position,id)+second_step_continuations(position,id)/2; value+=piece_color(id)==Color::White?mobility:-mobility;} return value;}
Value evaluate(const Position& position){const Value v=evaluate_white(position); return position.side_to_move()==Color::White?v:-v;}
}

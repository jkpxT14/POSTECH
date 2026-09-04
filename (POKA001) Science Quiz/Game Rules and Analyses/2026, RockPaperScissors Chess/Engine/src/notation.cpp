#include "notation.h"

#include <cctype>
#include <sstream>

namespace rpsc {
namespace {
const char* item_code(Item item) {
    switch (item) {
        case Item::None: return "";
        case Item::Push: return "Pu";
        case Item::RotateNorth: return "RoN";
        case Item::RotateSouth: return "RoS";
        case Item::RotateEast: return "RoE";
        case Item::RotateWest: return "RoW";
        case Item::RotateLeft: return "RoL";
        case Item::RotateRight: return "RoR";
        case Item::StepShort: return "StS";
        case Item::StepLong: return "StL";
    }
    return "";
}

bool parse_item(const std::string& code, Item& item) {
    if (code.empty()) item = Item::None;
    else if (code == "Pu") item = Item::Push;
    else if (code == "RoN") item = Item::RotateNorth;
    else if (code == "RoS") item = Item::RotateSouth;
    else if (code == "RoE") item = Item::RotateEast;
    else if (code == "RoW") item = Item::RotateWest;
    else if (code == "RoL") item = Item::RotateLeft;
    else if (code == "RoR") item = Item::RotateRight;
    else if (code == "StS") item = Item::StepShort;
    else if (code == "StL") item = Item::StepLong;
    else return false;
    return true;
}

bool parse_square_token(const std::string& text, std::size_t offset, Square& square) {
    if (offset + 2 > text.size()) return false;
    const char file = text[offset], rank = text[offset + 1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return false;
    square = make_square(file - 'a', rank - '1');
    return true;
}

void trim(std::string& text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.erase(text.begin());
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.pop_back();
}
}  // namespace

std::string square_name(Square square) {
    if (square == NoSquare) return "--";
    std::string out;
    out += char('a' + file_of(square));
    out += char('1' + rank_of(square));
    return out;
}

std::string piece_name(PieceId piece) {
    const int index = piece_index(piece);
    std::string out;
    out += index < 4 ? 'W' : 'B';
    out += char('1' + (index % 4));
    return out;
}

std::string format_move(const Move& move) {
    std::ostringstream out;
    out << piece_name(move.piece);
    if (move.item != Item::None) out << '[' << item_code(move.item) << ']';
    out << ": " << square_name(move.from());
    if (move.item == Item::Push) out << '>' << square_name(move.push_to);
    for (std::uint8_t i = 1; i < move.path_length; ++i) out << '-' << square_name(move.path[i]);
    return out.str();
}

std::string format_move(const Position& before, const Move& move) {
    Position position = before;
    UndoState undo;
    const MoveOutcome outcome = position.do_move(move, undo);
    std::string result = format_move(move);
    if (outcome.has_capture) result += " x" + piece_name(outcome.captured);
    if (outcome.reset) result += " Reset";
    return result;
}

bool parse_move(const Position& position, const std::string& input, Move& move) {
    std::string text = input;
    trim(text);
    // Native Engine parses one board move. Official Timeout is a game-controller event,
    // so a Timeout prefix is intentionally not accepted here.
    const auto colon = text.find(':');
    if (colon == std::string::npos || colon < 2) return false;

    std::string head = text.substr(0, colon);
    trim(head);
    std::string id = head, code;
    const auto open = head.find('[');
    if (open != std::string::npos) {
        const auto close = head.find(']', open + 1);
        if (close == std::string::npos || close + 1 != head.size()) return false;
        id = head.substr(0, open);
        code = head.substr(open + 1, close - open - 1);
    }

    PieceId piece = PieceId::W1;
    bool found = false;
    for (int i = 0; i < PieceCount; ++i) {
        auto candidate = static_cast<PieceId>(i);
        if (piece_name(candidate) == id) {
            piece = candidate;
            found = true;
            break;
        }
    }
    if (!found) return false;

    Item item;
    if (!parse_item(code, item)) return false;

    std::string path = text.substr(colon + 1);
    const auto suffix = path.find(" x");
    if (suffix != std::string::npos) path = path.substr(0, suffix);
    const auto reset = path.find(" Reset");
    if (reset != std::string::npos) path = path.substr(0, reset);
    trim(path);

    Move parsed;
    parsed.piece = piece;
    parsed.item = item;
    std::size_t pos = 0;
    Square start;
    if (!parse_square_token(path, pos, start)) return false;
    parsed.path[parsed.path_length++] = start;
    pos += 2;

    if (item == Item::Push) {
        if (pos >= path.size() || path[pos] != '>') return false;
        ++pos;
        if (!parse_square_token(path, pos, parsed.push_to)) return false;
        pos += 2;
    }

    while (pos < path.size()) {
        if (path[pos] != '-') return false;
        ++pos;
        Square square;
        if (!parse_square_token(path, pos, square)) return false;
        if (parsed.path_length >= MaxMoveSquares) return false;
        parsed.path[parsed.path_length++] = square;
        pos += 2;
    }

    if (!position.is_legal_path(parsed)) return false;
    move = parsed;
    return true;
}

std::string format_pv(const Position& start, const std::vector<Move>& pv, int m_number) {
    Position position = start;
    std::ostringstream out;
    int number = m_number;
    for (std::size_t i = 0; i < pv.size(); ++i) {
        if (i) out << ' ';
        if (position.side_to_move() == Color::White) out << 'M' << number << ". ";
        else out << 'M' << number << "... ";
        out << format_move(position, pv[i]);
        UndoState undo;
        position.do_move(pv[i], undo);
        if (position.side_to_move() == Color::White) ++number;
    }
    return out.str();
}
}  // namespace rpsc

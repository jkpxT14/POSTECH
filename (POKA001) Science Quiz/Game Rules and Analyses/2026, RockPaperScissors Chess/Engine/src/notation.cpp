#include "notation.h"

#include <cctype>
#include <sstream>

namespace rpsc {

std::string square_name(Square square) {
    if (square == NoSquare) return "--";

    std::string result;
    result += char('a' + file_of(square));
    result += char('1' + rank_of(square));
    return result;
}

std::string piece_name(PieceId piece) {
    const int index = piece_index(piece);
    std::string result;
    result += index < 4 ? 'W' : 'B';
    result += char('1' + (index % 4));
    return result;
}

std::string format_move(const Move& move) {
    std::ostringstream out;
    out << piece_name(move.piece) << ": ";
    for (std::uint8_t i = 0; i < move.path_length; ++i) {
        if (i) out << '-';
        out << square_name(move.path[i]);
    }
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

bool parse_move(const Position& position, const std::string& text, Move& move) {
    const auto colon = text.find(':');
    if (colon == std::string::npos || colon < 2) return false;

    std::string id = text.substr(0, colon);
    while (!id.empty() && std::isspace(static_cast<unsigned char>(id.back()))) id.pop_back();

    PieceId piece = PieceId::W1;
    bool found = false;
    for (int i = 0; i < PieceCount; ++i) {
        const auto candidate = static_cast<PieceId>(i);
        if (piece_name(candidate) == id) {
            piece = candidate;
            found = true;
            break;
        }
    }
    if (!found) return false;

    std::string path = text.substr(colon + 1);
    const auto capture = path.find(" x");
    if (capture != std::string::npos) path = path.substr(0, capture);
    const auto reset = path.find(" Reset");
    if (reset != std::string::npos) path = path.substr(0, reset);

    std::istringstream stream(path);
    std::string token;
    Move parsed;
    parsed.piece = piece;

    while (std::getline(stream, token, '-')) {
        while (!token.empty() && std::isspace(static_cast<unsigned char>(token.front())))
            token.erase(token.begin());
        while (!token.empty() && std::isspace(static_cast<unsigned char>(token.back())))
            token.pop_back();

        if (token.size() != 2 || token[0] < 'a' || token[0] > 'h' || token[1] < '1' ||
            token[1] > '8')
            return false;
        if (parsed.path_length >= MaxMoveSquares) return false;

        parsed.path[parsed.path_length++] = make_square(token[0] - 'a', token[1] - '1');
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
        if (position.side_to_move() == Color::White)
            out << 'M' << number << ". ";
        else
            out << 'M' << number << "... ";

        out << format_move(position, pv[i]);
        UndoState undo;
        position.do_move(pv[i], undo);
        if (position.side_to_move() == Color::White) ++number;
    }
    return out.str();
}

}  // namespace rpsc

#include "Move.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <ostream>

namespace prs {
namespace {

constexpr std::string_view kWhitespace = " \t\r\n";

}  // namespace

std::string_view toString(Move move) {
    switch (move) {
        case Move::Rock: return "Rock";
        case Move::Paper: return "Paper";
        case Move::Scissors: return "Scissors";
    }
    return "Unknown move";
}

std::string normalize(std::string_view text) {
    const std::size_t first = text.find_first_not_of(kWhitespace);
    if (first == std::string_view::npos) {
        return {};
    }
    const std::size_t last = text.find_last_not_of(kWhitespace);

    std::string result(text.substr(first, last - first + 1));
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::optional<Move> parseMove(std::string_view text) {
    const std::string input = normalize(text);
    for (const Move move : kAllMoves) {
        const std::string name = normalize(toString(move));
        if (input == name || input == name.substr(0, 1)) {
            return move;
        }
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& out, Move move) { return out << toString(move); }

}  // namespace prs

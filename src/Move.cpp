#include "Move.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <ostream>

namespace imc {
namespace {

// "\r" is included because text with Windows line endings keeps it at the end of every line read
// with std::getline.
constexpr std::string_view kWhitespace = " \t\r\n";

}  // namespace

std::string_view toString(Move move) {
    switch (move) {
        case Move::Rock: return "Rock";
        case Move::Paper: return "Paper";
        case Move::Scissors: return "Scissors";
    }
    
    // Only reached with an invalid enum value; keeps every path returning a value.
    return "Unknown move";
}

std::string normalize(std::string_view text) {
    const std::size_t first = text.find_first_not_of(kWhitespace);
    if (first == std::string_view::npos) {
        return {};  // empty or whitespace only
    }
    const std::size_t last = text.find_last_not_of(kWhitespace);

    std::string result(text.substr(first, last - first + 1));
    // std::tolower is undefined for negative values, so each char is passed as unsigned char.
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::optional<Move> parseMove(std::string_view text) {
    const std::string input = normalize(text);
    // The accepted words come from toString(), so the input always matches the displayed names.
    for (const Move move : kAllMoves) {
        const std::string name = normalize(toString(move));
        if (input == name || input == name.substr(0, 1)) {
            return move;
        }
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& out, Move move) { return out << toString(move); }

}  // namespace imc

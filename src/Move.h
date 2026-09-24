#pragma once

#include <array>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

namespace prs {

// A hand shown by a player: a fist, an open hand, or index and middle finger.
enum class Move { Rock, Paper, Scissors };

inline constexpr std::array<Move, 3> kAllMoves{Move::Rock, Move::Paper, Move::Scissors};

std::string_view toString(Move move);

// Reads a move typed by a player: "rock", "R" or " Paper ". Empty if the text is not a move.
std::optional<Move> parseMove(std::string_view text);

// A trimmed, lower-cased copy, so that typed text can be compared.
std::string normalize(std::string_view text);

std::ostream& operator<<(std::ostream& out, Move move);

}  // namespace prs

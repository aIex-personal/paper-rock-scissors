#pragma once

#include <array>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

namespace imc {

/// A hand a player can show: a fist (Rock), an open hand (Paper), or the index and middle finger
enum class Move { Rock, Paper, Scissors };

/// Every move, in a fixed order. Used to loop over all moves and to pick one at random.
inline constexpr std::array<Move, 3> kAllMoves{Move::Rock, Move::Paper, Move::Scissors};

/// The display name of a move, e.g. "Rock".
std::string_view toString(Move move);

/// Reads a move typed by a player. Accepts the full name or its first letter, in any case and with
/// surrounding whitespace ("rock", "R", " Paper "). Returns std::nullopt for anything else.
std::optional<Move> parseMove(std::string_view text);

/// A copy of `text` without surrounding whitespace and in lower case, so that typed input can be
/// compared with known words.
std::string normalize(std::string_view text);

/// Writes the display name of a move, so moves can be printed directly.
std::ostream& operator<<(std::ostream& out, Move move);

}  // namespace imc
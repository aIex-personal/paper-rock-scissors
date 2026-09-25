#pragma once

#include <array>
#include <iosfwd>
#include <string>
#include <string_view>

#include "Move.h"

namespace imc {

/// How a round (or a whole game) ended, seen from the first player's side.
enum class Outcome { Win, Loss, Draw };

/// One rule of the game, e.g. "Paper wraps Rock": `winner` beats `loser`, described by `verb`.
struct Rule {
    Move winner;
    std::string_view verb;
    Move loser;
};

/// The rules of the game, stored as data instead of if/else statements. Judging a round,
/// explaining its result and printing the rules all use this one table.
inline constexpr std::array<Rule, 3> kRules{Rule{Move::Paper, "wraps", Move::Rock},
                                            Rule{Move::Rock, "blunts", Move::Scissors},
                                            Rule{Move::Scissors, "cuts", Move::Paper}};

/// How `move` does against `opponentMove`: Win, Loss or Draw.
Outcome judge(Move move, Move opponentMove);

/// The rule that decided the round, e.g. "Rock blunts Scissors", in either order of the moves.
/// Empty when both moves are the same.
std::string describe(Move move, Move opponentMove);

/// Writes "Win", "Loss" or "Draw". Mainly used so that failing tests print readable values.
std::ostream& operator<<(std::ostream& out, Outcome outcome);

}  // namespace imc

#pragma once

#include <array>
#include <iosfwd>
#include <string>
#include <string_view>

#include "Move.h"

namespace imc {

// How a round ended, seen from the first player.
enum class Outcome { Win, Loss, Draw };

// One rule of the game: "Paper wraps Rock".
struct Rule {
    Move winner;
    std::string_view verb;
    Move loser;
};

// The rules as data instead of if/else statements, so that judging a round, explaining it and
// printing the rules all use these three lines. A variant of the game needs new moves and a new
// list here, nothing else.
inline constexpr std::array<Rule, 3> kRules{Rule{Move::Paper, "wraps", Move::Rock},
                                            Rule{Move::Rock, "blunts", Move::Scissors},
                                            Rule{Move::Scissors, "cuts", Move::Paper}};

// How `move` fares against `opponentMove`.
Outcome judge(Move move, Move opponentMove);

// The rule that decided the round, e.g. "Rock blunts Scissors"; empty when both moves are equal.
std::string describe(Move move, Move opponentMove);

std::ostream& operator<<(std::ostream& out, Outcome outcome);

}  // namespace imc

#pragma once

#include <optional>

#include "Player.h"
#include "Rules.h"

namespace imc {

/// What both players showed in one round, and how it ended for the first player.
struct RoundResult {
    Move firstMove;
    Move secondMove;
    Outcome outcome;
};

/// How many rounds each player has won, counted from the first player's side.
struct Score {
    int firstPlayerWins = 0;
    int secondPlayerWins = 0;
    int draws = 0;

    int roundsPlayed() const { return firstPlayerWins + secondPlayerWins + draws; }
    Outcome overallOutcome() const;
};

/// Plays rounds between two players and keeps the score. It has no input or output of its own,
/// so the same game works behind a console, a user interface or a test.
class Game {
public:
    // The players must live longer than the game; it only refers to them.
    Game(Player& first, Player& second) : first_(first), second_(second) {}

    // Asks both players for a move and judges the round. Empty when a player stops playing.
    std::optional<RoundResult> playRound();

    const Player& firstPlayer() const { return first_; }
    const Player& secondPlayer() const { return second_; }
    const Score& score() const { return score_; }

private:
    Player& first_;
    Player& second_;
    Score score_;
};

}  // namespace imc

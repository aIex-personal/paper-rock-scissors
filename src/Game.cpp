#include "Game.h"

namespace imc {

Outcome Score::overallOutcome() const {
    if (firstPlayerWins > secondPlayerWins) {
        return Outcome::Win;
    }
    if (firstPlayerWins < secondPlayerWins) {
        return Outcome::Loss;
    }
    return Outcome::Draw;
}

std::optional<RoundResult> Game::playRound() {
    // Neither player sees the other's move, so the round is as simultaneous as on a table.
    const std::optional<Move> firstMove = first_.chooseMove();
    if (!firstMove) {
        return std::nullopt;
    }
    const std::optional<Move> secondMove = second_.chooseMove();
    if (!secondMove) {
        return std::nullopt;
    }

    const RoundResult result{*firstMove, *secondMove, judge(*firstMove, *secondMove)};
    switch (result.outcome) {
        case Outcome::Win: ++score_.firstPlayerWins; break;
        case Outcome::Loss: ++score_.secondPlayerWins; break;
        case Outcome::Draw: ++score_.draws; break;
    }
    return result;
}

}  // namespace imc

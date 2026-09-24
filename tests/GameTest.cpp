#include <optional>

#include <gtest/gtest.h>

#include "Game.h"
#include "TestHelpers.h"

namespace imc {
namespace {

using test::ScriptedPlayer;

TEST(ScoreTest, StartsAtZeroAndCountsEveryRound) {
    Score score;
    EXPECT_EQ(score.roundsPlayed(), 0);
    EXPECT_EQ(score.overallOutcome(), Outcome::Draw);

    score.firstPlayerWins = 2;
    score.secondPlayerWins = 1;
    score.draws = 1;
    EXPECT_EQ(score.roundsPlayed(), 4);
    EXPECT_EQ(score.overallOutcome(), Outcome::Win);
}

TEST(ScoreTest, OverallWinnerIsWhoeverWonMoreRounds) {
    Score score;
    score.draws = 5;
    EXPECT_EQ(score.overallOutcome(), Outcome::Draw);

    score.secondPlayerWins = 1;
    EXPECT_EQ(score.overallOutcome(), Outcome::Loss);

    score.firstPlayerWins = 2;
    EXPECT_EQ(score.overallOutcome(), Outcome::Win);
}

TEST(GameTest, PlaysARoundAndJudgesItFromTheFirstPlayersPointOfView) {
    ScriptedPlayer first("First", {Move::Rock});
    ScriptedPlayer second("Second", {Move::Scissors});
    Game game(first, second);

    const std::optional<RoundResult> result = game.playRound();

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->firstMove, Move::Rock);
    EXPECT_EQ(result->secondMove, Move::Scissors);
    EXPECT_EQ(result->outcome, Outcome::Win);
}

TEST(GameTest, KeepsTheScoreOverSeveralRounds) {
    ScriptedPlayer first("First", {Move::Rock, Move::Rock, Move::Paper});
    ScriptedPlayer second("Second", {Move::Scissors, Move::Paper, Move::Paper});
    Game game(first, second);

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(game.playRound().has_value());
    }

    EXPECT_EQ(game.score().firstPlayerWins, 1);
    EXPECT_EQ(game.score().secondPlayerWins, 1);
    EXPECT_EQ(game.score().draws, 1);
    EXPECT_EQ(first.timesAsked(), 3);
    EXPECT_EQ(second.timesAsked(), 3);
}

TEST(GameTest, StopsWhenTheFirstPlayerStopsWithoutAskingTheSecondPlayer) {
    ScriptedPlayer first("First", {});
    ScriptedPlayer second("Second", {Move::Rock});
    Game game(first, second);

    EXPECT_FALSE(game.playRound().has_value());
    EXPECT_EQ(second.timesAsked(), 0);
    EXPECT_EQ(game.score().roundsPlayed(), 0);
}

TEST(GameTest, StopsWhenTheSecondPlayerStops) {
    ScriptedPlayer first("First", {Move::Rock});
    ScriptedPlayer second("Second", {});
    Game game(first, second);

    EXPECT_FALSE(game.playRound().has_value());
    EXPECT_EQ(game.score().roundsPlayed(), 0);
}

TEST(GameTest, KnowsItsPlayers) {
    ScriptedPlayer first("First", {});
    ScriptedPlayer second("Second", {});
    const Game game(first, second);

    EXPECT_EQ(game.firstPlayer().name(), "First");
    EXPECT_EQ(game.secondPlayer().name(), "Second");
}

}  // namespace
}  // namespace imc

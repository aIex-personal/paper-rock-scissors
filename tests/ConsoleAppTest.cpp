#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ConsoleApp.h"
#include "Game.h"
#include "Player.h"
#include "TestHelpers.h"

namespace imc {
namespace {

using test::countOccurrences;
using test::ScriptedPlayer;
using ::testing::HasSubstr;
using ::testing::Not;

// A complete console session: the player types `typed` and plays against a computer whose moves
// are known in advance, so the whole game is predictable.
struct Session {
    Session(const std::string& typed, std::vector<Move> computerMoves)
        : input(typed), computer("Computer", std::move(computerMoves)) {}

    // Runs the application and returns everything it wrote to the console.
    std::string run() {
        Game game(human, computer);
        ConsoleApp(input, output).run(game);
        return output.str();
    }

    std::istringstream input;
    std::ostringstream output;
    HumanPlayer human{"You", input, output};
    ScriptedPlayer computer;
};

// What the player types is not echoed into the captured output (on a real console the Enter key
// ends the line), so every prompt is followed directly by the next output.
TEST(ConsoleAppTest, PlaysACompleteGame) {
    Session session("2\nrock\nscissors\n", {Move::Scissors, Move::Scissors});

    EXPECT_EQ(session.run(),
              "=== Paper - Rock - Scissors ===\n"
              "Rules:\n"
              "  Paper wraps Rock\n"
              "  Rock blunts Scissors\n"
              "  Scissors cuts Paper\n"
              "\n"
              "How many rounds would you like to play? \n"
              "--- Round 1 of 2 ---\n"
              "Choose your move - [r]ock, [p]aper, [s]cissors or [q]uit: "
              "You played Rock, Computer played Scissors.\n"
              "Rock blunts Scissors - You won this round!\n"
              "Score: You 1 - 0 Computer (draws: 0)\n"
              "\n"
              "--- Round 2 of 2 ---\n"
              "Choose your move - [r]ock, [p]aper, [s]cissors or [q]uit: "
              "You played Scissors, Computer played Scissors.\n"
              "It's a draw.\n"
              "Score: You 1 - 0 Computer (draws: 1)\n"
              "\n"
              "=== Final result after 2 rounds ===\n"
              "Score: You 1 - 0 Computer (draws: 1)\n"
              "You won the game!\n"
              "\n"
              "Thanks for playing!\n");
}

TEST(ConsoleAppTest, PlaysExactlyTheChosenNumberOfRounds) {
    // More moves are available than needed: the game must still stop after 3 rounds.
    Session session("3\nrock\nrock\nrock\nrock\nrock\n",
                    {Move::Rock, Move::Rock, Move::Rock, Move::Rock, Move::Rock});

    const std::string output = session.run();

    EXPECT_EQ(session.computer.timesAsked(), 3);
    EXPECT_EQ(countOccurrences(output, "--- Round"), 3);
    EXPECT_THAT(output, HasSubstr("=== Final result after 3 rounds ===\n"
                                  "Score: You 0 - 0 Computer (draws: 3)\n"
                                  "The game is a draw.\n"));
}

TEST(ConsoleAppTest, AnnouncesTheComputerAsWinner) {
    Session session("1\npaper\n", {Move::Scissors});

    const std::string output = session.run();

    EXPECT_THAT(output, HasSubstr("Scissors cuts Paper - Computer won this round!\n"));
    EXPECT_THAT(output, HasSubstr("=== Final result after 1 round ===\n"
                                  "Score: You 0 - 1 Computer (draws: 0)\n"
                                  "Computer won the game!\n"));
}

TEST(ConsoleAppTest, AsksAgainUntilTheNumberOfRoundsIsValid) {
    Session session("abc\n0\n-2\n2.5\n\n99999999999999999999\n1\nrock\n", {Move::Paper});

    const std::string output = session.run();

    EXPECT_EQ(countOccurrences(output, "Please enter a whole number greater than 0: "), 6);
    EXPECT_THAT(output, HasSubstr("--- Round 1 of 1 ---"));
    EXPECT_EQ(session.computer.timesAsked(), 1);
}

TEST(ConsoleAppTest, StopsEarlyWhenThePlayerQuits) {
    Session session("3\nrock\nq\nrock\nrock\n", {Move::Paper, Move::Paper, Move::Paper});

    const std::string output = session.run();

    EXPECT_EQ(session.computer.timesAsked(), 1);
    EXPECT_THAT(output, HasSubstr("Game stopped before all rounds were played.\n"));
    EXPECT_THAT(output, HasSubstr("=== Final result after 1 round ===\n"));
    EXPECT_THAT(output, HasSubstr("Thanks for playing!\n"));
}

TEST(ConsoleAppTest, StopsEarlyWhenTheInputEnds) {
    Session session("3\nrock\n", {Move::Rock, Move::Rock, Move::Rock});

    const std::string output = session.run();

    EXPECT_THAT(output, HasSubstr("Game stopped before all rounds were played.\n"));
    EXPECT_THAT(output, HasSubstr("=== Final result after 1 round ===\n"));
}

TEST(ConsoleAppTest, ShowsNoFinalResultWhenNoRoundWasPlayed) {
    Session session("2\nquit\nrock\nrock\n", {Move::Rock, Move::Rock});

    const std::string output = session.run();

    EXPECT_THAT(output, Not(HasSubstr("Final result")));
    EXPECT_THAT(output, HasSubstr("Thanks for playing!\n"));
}

TEST(ConsoleAppTest, EndsCleanlyWhenThereIsNoInputAtAll) {
    Session session("", {});

    const std::string output = session.run();

    EXPECT_EQ(session.computer.timesAsked(), 0);
    EXPECT_THAT(output, Not(HasSubstr("--- Round")));
    EXPECT_THAT(output, HasSubstr("Thanks for playing!\n"));
}

}  // namespace
}  // namespace imc

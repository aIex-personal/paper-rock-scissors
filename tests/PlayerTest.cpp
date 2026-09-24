#include <initializer_list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "Player.h"
#include "TestHelpers.h"

namespace imc {
namespace {

using test::countOccurrences;

// A human player whose keyboard input is given in advance and whose output is captured.
struct ConsoleHuman {
    explicit ConsoleHuman(const std::string& typed) : input(typed) {}

    std::istringstream input;
    std::ostringstream output;
    HumanPlayer player{"You", input, output};
};

TEST(HumanPlayerTest, AsksForAMoveAndReadsIt) {
    ConsoleHuman human("paper\n");

    EXPECT_EQ(human.player.chooseMove(), Move::Paper);
    EXPECT_EQ(human.output.str(), "Choose your move - [r]ock, [p]aper, [s]cissors or [q]uit: ");
}

TEST(HumanPlayerTest, ReadsOneMovePerLine) {
    ConsoleHuman human("R\n p \nScissors\r\n");

    EXPECT_EQ(human.player.chooseMove(), Move::Rock);
    EXPECT_EQ(human.player.chooseMove(), Move::Paper);
    EXPECT_EQ(human.player.chooseMove(), Move::Scissors);
}

TEST(HumanPlayerTest, AsksAgainAfterAnInvalidMove) {
    ConsoleHuman human("banana\n\nscissors\n");

    EXPECT_EQ(human.player.chooseMove(), Move::Scissors);
    EXPECT_EQ(countOccurrences(human.output.str(), "Sorry, that is not a valid move."), 2);
    EXPECT_EQ(countOccurrences(human.output.str(), "Choose your move"), 3);
}

TEST(HumanPlayerTest, StopsPlayingWhenThePlayerQuits) {
    // A valid move follows the quit command, so the test fails if the command is not recognized.
    for (const char* typed : {"q\nrock\n", "quit\nrock\n", "  QUIT \nrock\n"}) {
        ConsoleHuman human(typed);
        EXPECT_FALSE(human.player.chooseMove().has_value()) << "typed: " << typed;
    }
}

TEST(HumanPlayerTest, StopsPlayingWhenTheInputEnds) {
    ConsoleHuman human("");  // Ctrl+Z on Windows, Ctrl+D on Linux and macOS
    EXPECT_FALSE(human.player.chooseMove().has_value());
}

std::vector<Move> chooseMoves(ComputerPlayer& computer, int count) {
    std::vector<Move> moves;
    for (int i = 0; i < count; ++i) {
        moves.push_back(computer.chooseMove().value());
    }
    return moves;
}

TEST(ComputerPlayerTest, NeverStopsPlaying) {
    ComputerPlayer computer("Computer");
    for (int i = 0; i < 100; ++i) {
        ASSERT_TRUE(computer.chooseMove().has_value());
    }
}

TEST(ComputerPlayerTest, PlaysTheSameMovesWhenGivenTheSameSeed) {
    ComputerPlayer first("First", 12345);
    ComputerPlayer second("Second", 12345);
    EXPECT_EQ(chooseMoves(first, 100), chooseMoves(second, 100));
}

// The moves for a seed depend on the standard library, so only the distribution is checked. With
// 3000 fair draws each move is expected about 1000 times, and the bounds are more than seven
// standard deviations away, so a correct implementation will not fail by chance.
TEST(ComputerPlayerTest, ChoosesEveryMoveAboutEquallyOften) {
    ComputerPlayer computer("Computer", 2026);
    std::map<Move, int> timesChosen;
    for (const Move move : chooseMoves(computer, 3000)) {
        ++timesChosen[move];
    }

    for (const Move move : kAllMoves) {
        EXPECT_GT(timesChosen[move], 800) << move;
        EXPECT_LT(timesChosen[move], 1200) << move;
    }
}

}  // namespace
}  // namespace imc

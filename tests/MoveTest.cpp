#include <initializer_list>
#include <sstream>

#include <gtest/gtest.h>

#include "Move.h"

namespace imc {
namespace {

TEST(MoveTest, HasANameForEveryMove) {
    EXPECT_EQ(toString(Move::Rock), "Rock");
    EXPECT_EQ(toString(Move::Paper), "Paper");
    EXPECT_EQ(toString(Move::Scissors), "Scissors");
}

TEST(MoveTest, CanBeWrittenToAStream) {
    std::ostringstream out;
    out << Move::Rock << ' ' << Move::Paper << ' ' << Move::Scissors;
    EXPECT_EQ(out.str(), "Rock Paper Scissors");
}

TEST(NormalizeTest, TrimsWhitespaceAndLowerCases) {
    EXPECT_EQ(normalize("rock"), "rock");
    EXPECT_EQ(normalize("  RoCk\t\r\n"), "rock");  // "\r" is left over from Windows line endings
    EXPECT_EQ(normalize(" Rock Paper "), "rock paper");
    EXPECT_EQ(normalize(" \t\r\n"), "");
}

TEST(ParseMoveTest, AcceptsFullNamesAndFirstLetters) {
    EXPECT_EQ(parseMove("rock"), Move::Rock);
    EXPECT_EQ(parseMove("paper"), Move::Paper);
    EXPECT_EQ(parseMove("scissors"), Move::Scissors);
    EXPECT_EQ(parseMove("r"), Move::Rock);
    EXPECT_EQ(parseMove("p"), Move::Paper);
    EXPECT_EQ(parseMove("s"), Move::Scissors);
}

TEST(ParseMoveTest, IgnoresCaseAndSurroundingWhitespace) {
    EXPECT_EQ(parseMove("ROCK"), Move::Rock);
    EXPECT_EQ(parseMove("  Paper\r"), Move::Paper);
    EXPECT_EQ(parseMove("\tS "), Move::Scissors);
}

TEST(ParseMoveTest, ReadsBackTheNameOfEveryMove) {
    for (const Move move : kAllMoves) {
        EXPECT_EQ(parseMove(toString(move)), move);
    }
}

TEST(ParseMoveTest, RejectsEverythingElse) {
    for (const char* input : {"", "   ", "x", "ro", "rocks", "rock paper", "1", "q", "lizard"}) {
        EXPECT_FALSE(parseMove(input).has_value()) << "input: \"" << input << '"';
    }
}

}  // namespace
}  // namespace imc

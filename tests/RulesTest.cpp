#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "Rules.h"

namespace imc {
namespace {

TEST(RulesTest, AreTheThreeRulesOfTheAssignment) {
    std::vector<std::string> lines;
    for (const Rule& rule : kRules) {
        lines.push_back(describe(rule.winner, rule.loser));
    }
    EXPECT_EQ(lines, (std::vector<std::string>{"Paper wraps Rock", "Rock blunts Scissors",
                                               "Scissors cuts Paper"}));
}

TEST(RulesTest, DescribeTellsHowTheRoundWasDecided) {
    EXPECT_EQ(describe(Move::Paper, Move::Rock), "Paper wraps Rock");
    EXPECT_EQ(describe(Move::Rock, Move::Paper), "Paper wraps Rock");
    EXPECT_EQ(describe(Move::Rock, Move::Scissors), "Rock blunts Scissors");
    EXPECT_EQ(describe(Move::Scissors, Move::Paper), "Scissors cuts Paper");
}

TEST(RulesTest, DescribeSaysNothingAboutADraw) {
    for (const Move move : kAllMoves) {
        EXPECT_EQ(describe(move, move), "") << move;
    }
}

TEST(OutcomeTest, CanBeWrittenToAStream) {
    std::ostringstream out;
    out << Outcome::Win << ' ' << Outcome::Loss << ' ' << Outcome::Draw;
    EXPECT_EQ(out.str(), "Win Loss Draw");
}

struct RoundCase {
    Move move;
    Move opponentMove;
    Outcome expected;
};

// Lets GoogleTest print the parameters of a failing test.
std::ostream& operator<<(std::ostream& out, const RoundCase& roundCase) {
    return out << roundCase.move << " vs " << roundCase.opponentMove;
}

// Gives each test case a readable name, e.g. "RockVsScissors".
std::string testName(const ::testing::TestParamInfo<RoundCase>& info) {
    return std::string(toString(info.param.move)) + "Vs" +
           std::string(toString(info.param.opponentMove));
}

// All 3 x 3 combinations of moves, which together specify judge() completely.
// clang-format off
const std::vector<RoundCase> kAllRoundCases = {
    // move          opponent's move   outcome for the player of `move`
    {Move::Rock,     Move::Rock,       Outcome::Draw},
    {Move::Rock,     Move::Paper,      Outcome::Loss},
    {Move::Rock,     Move::Scissors,   Outcome::Win},
    {Move::Paper,    Move::Rock,       Outcome::Win},
    {Move::Paper,    Move::Paper,      Outcome::Draw},
    {Move::Paper,    Move::Scissors,   Outcome::Loss},
    {Move::Scissors, Move::Rock,       Outcome::Loss},
    {Move::Scissors, Move::Paper,      Outcome::Win},
    {Move::Scissors, Move::Scissors,   Outcome::Draw},
};
// clang-format on

class JudgeTest : public ::testing::TestWithParam<RoundCase> {};

TEST_P(JudgeTest, JudgesTheRound) {
    const RoundCase& roundCase = GetParam();
    EXPECT_EQ(judge(roundCase.move, roundCase.opponentMove), roundCase.expected);
}

INSTANTIATE_TEST_SUITE_P(AllMoveCombinations, JudgeTest, ::testing::ValuesIn(kAllRoundCases),
                         testName);

}  // namespace
}  // namespace imc

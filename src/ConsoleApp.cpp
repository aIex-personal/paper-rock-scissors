#include "ConsoleApp.h"

#include <istream>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>

#include "Game.h"

namespace imc {
namespace {

// A whole number greater than zero: " 5 " gives 5, while "0", "-2", "2.5" and "5x" give nothing.
std::optional<int> parsePositiveNumber(const std::string& text) {
    std::istringstream input(text);
    int number = 0;
    char extra = 0;
    if (input >> number && !(input >> extra) && number > 0) {
        return number;
    }
    return std::nullopt;
}

// Asks until the player enters a valid number of rounds. Empty when the input ends.
std::optional<int> askNumberOfRounds(std::istream& in, std::ostream& out) {
    out << "How many rounds would you like to play? ";
    std::string line;
    while (std::getline(in, line)) {
        if (const std::optional<int> rounds = parsePositiveNumber(line)) {
            return rounds;
        }
        out << "Please enter a whole number greater than 0: ";
    }
    return std::nullopt;
}

void printWelcome(std::ostream& out) {
    out << "=== Paper - Rock - Scissors ===\n"
        << "Rules:\n";
    for (const Rule& rule : kRules) {
        out << "  " << describe(rule.winner, rule.loser) << '\n';
    }
    out << '\n';
}

void printScore(std::ostream& out, const Game& game) {
    const Score& score = game.score();
    out << "Score: " << game.firstPlayer().name() << ' ' << score.firstPlayerWins << " - "
        << score.secondPlayerWins << ' ' << game.secondPlayer().name();
    out << " (draws: " << score.draws << ")\n";
}

void printRound(std::ostream& out, const Game& game, const RoundResult& result) {
    const std::string& first = game.firstPlayer().name();
    const std::string& second = game.secondPlayer().name();
    out << first << " played " << result.firstMove << ", " << second << " played "
        << result.secondMove << ".\n";

    if (result.outcome == Outcome::Draw) {
        out << "It's a draw.\n";
    } else {
        const std::string& winner = result.outcome == Outcome::Win ? first : second;
        out << describe(result.firstMove, result.secondMove) << " - " << winner
            << " won this round!\n";
    }
    printScore(out, game);
}

void playRounds(std::ostream& out, Game& game, int rounds) {
    for (int roundNumber = 1; roundNumber <= rounds; ++roundNumber) {
        out << "\n--- Round " << roundNumber << " of " << rounds << " ---\n";
        const std::optional<RoundResult> result = game.playRound();
        if (!result) {
            out << "\nGame stopped before all rounds were played.\n";
            return;
        }
        printRound(out, game, *result);
    }
}

void printFinalResult(std::ostream& out, const Game& game) {
    const Score& score = game.score();
    const int rounds = score.roundsPlayed();
    out << "\n=== Final result after " << rounds << (rounds == 1 ? " round" : " rounds")
        << " ===\n";
    printScore(out, game);
    switch (score.overallOutcome()) {
        case Outcome::Win: out << game.firstPlayer().name() << " won the game!\n"; break;
        case Outcome::Loss: out << game.secondPlayer().name() << " won the game!\n"; break;
        case Outcome::Draw: out << "The game is a draw.\n"; break;
    }
}

}  // namespace

ConsoleApp::ConsoleApp(std::istream& in, std::ostream& out) : in_(in), out_(out) {}

void ConsoleApp::run(Game& game) {
    printWelcome(out_);
    if (const std::optional<int> rounds = askNumberOfRounds(in_, out_)) {
        playRounds(out_, game, *rounds);
    }
    if (game.score().roundsPlayed() > 0) {
        printFinalResult(out_, game);
    }
    out_ << "\nThanks for playing!\n";
}

}  // namespace imc

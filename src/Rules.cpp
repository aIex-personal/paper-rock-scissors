#include "Rules.h"

#include <algorithm>
#include <ostream>

namespace imc {
namespace {

// Finds the rule that applies to the two moves, whichever of them won. Returns nullptr when no
// rule applies, which with the current rules only happens when both moves are the same.
const Rule* findRule(Move move, Move opponentMove) {
    const auto rule = std::find_if(kRules.begin(), kRules.end(), [&](const Rule& candidate) {
        return (candidate.winner == move && candidate.loser == opponentMove) ||
               (candidate.winner == opponentMove && candidate.loser == move);
    });
    return rule == kRules.end() ? nullptr : &*rule;
}

}  // namespace

Outcome judge(Move move, Move opponentMove) {
    const Rule* rule = findRule(move, opponentMove);
    if (rule == nullptr) {
        return Outcome::Draw;
    }
    return rule->winner == move ? Outcome::Win : Outcome::Loss;
}

std::string describe(Move move, Move opponentMove) {
    const Rule* rule = findRule(move, opponentMove);
    if (rule == nullptr) {
        return {};
    }
    // Always written from the winner's side, e.g. "Rock blunts Scissors", never "Scissors ...".
    return std::string(toString(rule->winner)) + ' ' + std::string(rule->verb) + ' ' +
           std::string(toString(rule->loser));
}

// Only reached with an invalid enum value.
std::ostream& operator<<(std::ostream& out, Outcome outcome) {
    switch (outcome) {
        case Outcome::Win: return out << "Win";
        case Outcome::Loss: return out << "Loss";
        case Outcome::Draw: return out << "Draw";
    }
    return out << "Unknown outcome";
}

}  // namespace imc

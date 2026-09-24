#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Player.h"

namespace imc::test {

// A player with fixed moves, so that tests do not depend on chance or on typing. When its moves
// run out it stops playing, like a person who types "quit".
class ScriptedPlayer final : public Player {
public:
    ScriptedPlayer(std::string name, std::vector<Move> moves)
        : Player(std::move(name)), moves_(std::move(moves)) {}

    std::optional<Move> chooseMove() override {
        ++timesAsked_;
        if (nextMove_ == moves_.size()) {
            return std::nullopt;
        }
        return moves_[nextMove_++];
    }

    int timesAsked() const { return timesAsked_; }

private:
    std::vector<Move> moves_;
    std::size_t nextMove_ = 0;
    int timesAsked_ = 0;
};

// How often `part` occurs in `text`.
inline int countOccurrences(std::string_view text, std::string_view part) {
    if (part.empty()) {
        return 0;
    }
    int count = 0;
    for (std::size_t pos = text.find(part); pos != std::string_view::npos;
         pos = text.find(part, pos + part.size())) {
        ++count;
    }
    return count;
}

}  // namespace imc::test

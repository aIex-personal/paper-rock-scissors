#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <random>
#include <string>
#include <utility>

#include "Move.h"

namespace imc {

/// A player of the game. This is the Strategy pattern: Game asks for a move without knowing
/// whether a person, the computer or a test is answering. No move means "I stop playing".
class Player {
public:
    explicit Player(std::string name) : name_(std::move(name)) {}
    virtual ~Player() = default;

    // Players are used through references and are never copied (that would slice them).
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    /// The name shown in the round results and the score, e.g. "You" or "Computer".
    const std::string& name() const { return name_; }
    /// The move for the next round, or std::nullopt when the player stops playing.
    virtual std::optional<Move> chooseMove() = 0;

private:
    std::string name_;
};

/// Asks the person at the console, and asks again after invalid input.
/// The streams are injected so tests can use string streams instead of a real console.
class HumanPlayer final : public Player {
public:
    HumanPlayer(std::string name, std::istream& in, std::ostream& out);

    std::optional<Move> chooseMove() override;

private:
    std::istream& in_;
    std::ostream& out_;
};

// Picks one of the three moves at random. It never sees the opponent's move, so it cannot cheat.
class ComputerPlayer final : public Player {
public:
    explicit ComputerPlayer(std::string name);
    // Fixed seed: the same moves every run, which makes the tests repeatable.
    ComputerPlayer(std::string name, std::uint32_t seed);

    /// Always returns a move the computer never stops playing on its own.
    std::optional<Move> chooseMove() override;

private:
    std::mt19937 randomEngine_;
};

}  // namespace imc

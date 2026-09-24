#include "Player.h"

#include <cstddef>
#include <istream>
#include <ostream>

namespace imc {
namespace {

constexpr std::string_view kPrompt = "Choose your move - [r]ock, [p]aper, [s]cissors or [q]uit: ";

bool isQuitCommand(const std::string& input) { return input == "q" || input == "quit"; }

}  // namespace

HumanPlayer::HumanPlayer(std::string name, std::istream& in, std::ostream& out)
    : Player(std::move(name)), in_(in), out_(out) {}

std::optional<Move> HumanPlayer::chooseMove() {
    std::string line;
    while (true) {
        out_ << kPrompt;
        if (!std::getline(in_, line)) {
            // The input ended: Ctrl+Z on Windows, Ctrl+D on Linux and macOS.
            return std::nullopt;
        }
        if (const std::optional<Move> move = parseMove(line)) {
            return move;
        }
        if (isQuitCommand(normalize(line))) {
            return std::nullopt;
        }
        out_ << "Sorry, that is not a valid move.\n";
    }
}

ComputerPlayer::ComputerPlayer(std::string name)
    : ComputerPlayer(std::move(name), std::random_device{}()) {}

ComputerPlayer::ComputerPlayer(std::string name, std::uint32_t seed)
    : Player(std::move(name)), randomEngine_(seed) {}

std::optional<Move> ComputerPlayer::chooseMove() {
    std::uniform_int_distribution<std::size_t> anyMove(0, kAllMoves.size() - 1);
    return kAllMoves[anyMove(randomEngine_)];
}

}  // namespace imc

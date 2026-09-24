#pragma once

#include <iosfwd>

namespace imc {

class Game;

/// The console user interface: asks how many rounds to play, plays them and shows the results.
///
/// All console input/output of the game flow lives here, so the game logic knows nothing about
/// the console. The streams are injected, so the whole flow can be tested without a real console.
class ConsoleApp {
public:
    ConsoleApp(std::istream& in, std::ostream& out);

    /// Asks the user for the number of rounds n, plays n rounds of `game` and shows the final
    /// result. Stops early if a player stops playing (e.g. the user types "quit").
    void run(Game& game);

private:
    std::istream& in_;
    std::ostream& out_;
};

}  // namespace imc

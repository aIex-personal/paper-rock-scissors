#include <cstdlib>
#include <exception>
#include <iostream>

#include "ConsoleApp.h"
#include "Game.h"
#include "Player.h"

// The only place that creates the objects and connects them, so swapping a player or the user
// interface happens here and nowhere else.
int main() {
    try {
        imc::HumanPlayer human("You", std::cin, std::cout);
        imc::ComputerPlayer computer("Computer");
        imc::Game game(human, computer);

        imc::ConsoleApp app(std::cin, std::cout);
        app.run(game);
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "Unexpected error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}

# RPaper-Rock-Scissors

A console game of Paper-Rock-Scissors between a player and the computer, written in C++17.
The player chooses how many rounds to play. After n rounds the program shows the final
result and exits.

## How to play
 
- Enter the number of rounds (a whole number greater than 0).
- Each round, type `rock`, `paper` or `scissors`, or just the first letter. Case doesn't matter.
- Type `q` or `quit` to stop early. The score so far is still shown.

## Requirements
- CMake 3.16 or newer
- A C++17 compiler

I developed and tested it on Fedora 44 with GCC 16.2, CMake 4.3 and Ninja 1.13. I haven't tried it on Windows or macOS, but the code only uses standard C++17, so I don't expect any problems there.

## Build and run
```
cmake -S . -B build
cmake --build build
./build/assignment-IMC
```

The first configure downloads the GoogleTest library. If you don't have internet access, you can skip the tests or use a GoogleTest that is already installed:
```
cmake -S . -B build -DPRS_BUILD_TESTS=OFF              # game only
cmake -S . -B build -DPRS_USE_SYSTEM_GTEST=ON          # use an installed GoogleTest
```

### Tests
```
ctest --test-dir build --output-on-failure
```
The tests cover all nine move combinations, input parsing, scoring, quitting early, and complete console sessions with fixed moves, so the output can be checked word for word.

## Design

```
                   main.cpp  (creates all objects and connects them)
                       |
       ---------------------------------
       |                               |
       |                               |
  ConsoleApp  ------ plays --------   Game  ----- judges with ------  Rules
  (asks for n, plays n rounds,        (plays one round,               (who beats whom,
   prints the results)                 keeps the Score, no I/O)        stored as data)
       |                               |
       |                               | asks for moves
       |                               |
       |                     abstract class Player
       |                        |               |
       |                        |               |
       |                   HumanPlayer     ComputerPlayer
       |                  (reads moves)    (random move)
       |                        |
       |                        |
  ----------------------------------------
  |     console (std::cin, std::cout)    |
  ----------------------------------------
```

A few decisions shaped the code:
 
**Players are interchangeable.** `Player` is an abstract class with a single method,
`chooseMove()`. `Game` only calls that method and doesn't know whether a person, the computer or a
test is answering (the Strategy pattern). In the tests I use a `ScriptedPlayer` that plays a fixed
list of moves. A smarter computer opponent would just be another subclass.
 
**Dependencies are passed in.** `Game` receives its players, and `ConsoleApp` and `HumanPlayer`
receive the input and output streams, instead of creating them themselves. Only `main.cpp` wires up
the real `std::cin`, `std::cout` and a randomly seeded computer. This is what makes it possible to
test a whole game with string streams and no randomness.
 
**The game logic has no input or ouput.** Everything the user sees is in `ConsoleApp` and `HumanPlayer`.
`Game` just plays rounds and keeps the score, so it's easy to test and could sit behind a different
interface.
 
**The rules are data.** The three rules live in one table in `Rules.h`, e.g.
`{Paper, "wraps", Rock}`. The same table is used to judge a round, to explain the result
("Rock blunts Scissors") and to print the rules at the start.


## Assumptions
 
- The number of rounds n is asked once at startup and must be a positive whole number.
- The overall winner is whoever won more rounds. Equal wins means the game is a draw.
- If the player quits or the input ends, the game stops and shows the result of the rounds played.

## Libraries

The game only uses the C++ standard library. For the tests I used **GoogleTest**.

I chose a test framework instead of writing my own checks with `assert`, because a framework
already handles the things I would otherwise have to build myself: running each test on its own,
showing what was expected versus what actually happened when a test fails, and working with
`ctest` and IDEs. That let me focus on the game and the tests themselves.

I picked GoogleTest because it's the most common C++ test framework, so the tests should be easy
to read for anyone who has worked with C++ before. Parameterized tests let me check all nine move
combinations from one table, and matchers like `HasSubstr` keep the console output tests short.

To keep the dependency simple, CMake downloads a fixed version and checks its hash. GoogleTest is
only linked into the tests, and the game can be built without it using `-DPRS_BUILD_TESTS=OFF`.

## Possible extensions
 
- **More moves** (Rock-Paper-Scissors-Lizard-Spock). This means adding the moves to `Move`
  and the rules to `kRules`; the judging code stays the same. Two things would need attention:
  first-letter shortcuts would clash (Scissors and Spock), and a check that every pair of moves has
  exactly one rule would be worth adding.
# MultiThreaded-Ping-Pong
 a multithreaded ping pong game built using C++, SFML, and pthread
It aims to demonstrate the capabilities of multithreading alongside the functionality provided by the SFML library for graphics and audio.

Gameplay
Players: 2 players can play locally on the same machine.
Controls:
Player 1: W (up), S (down)
Player 2: Arrow Up (up), Arrow Down (down)
The game features a simple scoring system, where the first player to reach 5 points wins.
Installation
Prerequisites: Ensure you have the following installed:

C++ compiler (g++ recommended)
SFML library (version 2.5 or higher)
pthread (usually comes pre-installed with Linux)
Clone the Repository:

bash
git clone https://github.com/username/multithreaded-ping-pong.git  
cd multithreaded-ping-pong  
Compile the Code:

bash
g++ -o ping_pong main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread  
Run the Game:

bash
./ping_pong  
Usage
Upon launching the game, you can start playing immediately. The game will display the scores and the current state of play. Use the designated keys for each player to control the paddles.

Contributing
Contributions are welcome! Feel free to open issues or submit pull requests for any improvements or bug fixes.

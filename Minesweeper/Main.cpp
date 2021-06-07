#include "tileandboard.h"

std::mt19937 Random::random(time(0));
using namespace std;
unordered_map<string, sf::Texture> TextureManager::textures;

int main() {
    //getting the data from the CFG file to load a board's size and data
    bool firstClick = true;

    string width, height, mines;
    unsigned int boardHeight = 0;
    unsigned int boardWidth = 0;
    unsigned int boardMines = 0;

    ifstream inFs;
    inFs.open("boards/config.cfg");
    getline(inFs, width), getline(inFs, height), getline(inFs, mines);
    boardHeight = stoi(height), boardWidth = stoi(width), boardMines = stoi(mines);
    unsigned int screenWidth = boardWidth * 32;
    unsigned int screenHeight = (boardHeight * 32) + 88;

    //opening the window based off of the information
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Minesweeper");

    //making the board that will run the minesweeper game
    Board board(boardWidth, boardHeight, boardMines);
    board.distributeMines(boardMines);
    board.setAdjacents();
    while (window.isOpen()) {
        sf::Event event;

        vector<vector<Tile>>* tiles = board.returnTiles();

        //meat and potatoes of main - changing the board based on user input
        while (window.pollEvent(event)) {
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonReleased:

                // these next five if statements handle the bottom buttons of the board - the three tests, debug, and the reset (happpy face) button.
                if ((event.mouseButton.x >= ((boardWidth * 32) / 2 - 32)) & (event.mouseButton.x <= (boardWidth * 32) / 2 + 32)
                    & (event.mouseButton.y >= (boardHeight * 32)) & (event.mouseButton.y <= (boardHeight * 32 + 64))) {
                    board.reset();
                    board.distributeMines(boardMines);
                    board.setAdjacents();
                    if (board.isDebugging()) {
                        board.debugBoard();
                    }
                    firstClick = true;
                }
                if ((event.mouseButton.x >= ((boardWidth * 32) / 2 + 160)) & (event.mouseButton.x <= (boardWidth * 32) / 2 + 224)
                    & (event.mouseButton.y >= (boardHeight * 32)) & (event.mouseButton.y <= (boardHeight * 32 + 64))) {
                    board.loadFromFile(1);
                }
                if ((event.mouseButton.x >= ((boardWidth * 32) / 2 + 224)) & (event.mouseButton.x <= (boardWidth * 32) / 2 + 288)
                    & (event.mouseButton.y >= (boardHeight * 32)) & (event.mouseButton.y <= (boardHeight * 32 + 64))) {
                    board.loadFromFile(2);
                }
                if ((event.mouseButton.x >= ((boardWidth * 32) / 2 + 288)) & (event.mouseButton.x <= (boardWidth * 32) / 2 + 352)
                    & (event.mouseButton.y >= (boardHeight * 32)) & (event.mouseButton.y <= (boardHeight * 32 + 64))) {
                    board.loadFromFile(3);
                }
                if ((event.mouseButton.x >= ((boardWidth * 32) / 2 + 96)) & (event.mouseButton.x <= (boardWidth * 32) / 2 + 160)
                    & (event.mouseButton.y >= (boardHeight * 32)) & (event.mouseButton.y <= (boardHeight * 32 + 64))) {
                    board.debugSwitch();
                }

                /*event handling for clicking the left mouse button. this is the most important click, and depending on the state of the tile either
                * reveals the tile or ends the game (either win or lose!).
                */
                if (event.mouseButton.button == sf::Mouse::Left && !board.checkLock() & !board.checkLock()) {
                    for (int i = 0; i < tiles->size(); i++) {
                        for (int j = 0; j < tiles->at(i).size(); j++) {
                            if ((event.mouseButton.x >= tiles->at(i).at(j).getX()) & (event.mouseButton.y >= tiles->at(i).at(j).getY())) {
                                if ((event.mouseButton.x < tiles->at(i).at(j).getX() + 32) & (event.mouseButton.y < tiles->at(i).at(j).getY() + 32)
                                    & !(tiles->at(i).at(j).checkFlag())) {

                                    /*NEW CODE*/ /*Allows the program to redistribute mines until the first click is no longer a mine*/
                                    while (firstClick) {
                                        if ((tiles->at(i).at(j).checkMine()) || (!tiles->at(i).at(j).checkBlank())) {
                                            board.reset();
                                            board.distributeMines(boardMines);
                                            board.setAdjacents();
                                            if (board.isDebugging()) {
                                                board.debugBoard();
                                            }
                                        }
                                        else {
                                            firstClick = false;
                                        }
                                    }
                                    

                                    if (tiles->at(i).at(j).checkBlank() == true) {
                                        board.recursiveReveal(i, j);
                                    }
                                    tiles->at(i).at(j).reveal();
                                    if (tiles->at(i).at(j).checkMine()) {
                                        board.lose();
                                    }

                                }
                            }
                        }
                    }
                }

                /*event handling for clicking the right mouse button. this either places or removes a flag depending on the
                * previous state of the tile, unlesss the tile is locked (in which case nothing happens).
                */
                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (int i = 0; i < tiles->size(); i++) {
                        for (int j = 0; j < tiles->at(i).size(); j++) {
                            if ((event.mouseButton.x >= tiles->at(i).at(j).getX()) & (event.mouseButton.y >= tiles->at(i).at(j).getY()) & (!board.checkLock())) {
                                if ((event.mouseButton.x < tiles->at(i).at(j).getX() + 32) & (event.mouseButton.y < tiles->at(i).at(j).getY() + 32)) {
                                    if (!(tiles->at(i).at(j).isRevealed())) {
                                        if (tiles->at(i).at(j).checkFlag()) {
                                            tiles->at(i).at(j).removeFlag();
                                            board.minusFlag();
                                        }
                                        else {
                                            tiles->at(i).at(j).setFlag();
                                            board.plusFlag();
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }

        }
        board.checkWin();
        board.flagCounter();

        //displaying of the board for each individual frame (I chose Cyan because it looks the nicest :D )
        window.clear(sf::Color::Cyan);
        board.DisplayBoard(window);
        window.display();

    }
    TextureManager::Clear();
    return 0;
}
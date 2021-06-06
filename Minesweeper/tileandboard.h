#pragma once
#include "randomgen.h"

//TILE CLASS

class Tile {
	//basic properties of the tile, includes location, sprites, and booleans for various states.
	int x;
	int y;
	sf::Sprite background;
	sf::Sprite sprite;
	sf::Sprite foreground;
	sf::Sprite flag;
	bool isMine;
	bool hasFlag;
	bool revealed;
	bool isBlank = false;
	bool isDebug;

public:
	vector<Tile*> adjacentTiles;
	//constructor
	Tile(int x, int y, bool mine) {
		this->x = x;
		this->y = y;
		this->isMine = mine;
		background = sf::Sprite(TextureManager::GetTexture("tile_revealed"));
		foreground = sf::Sprite(TextureManager::GetTexture("tile_hidden"));
		flag = sf::Sprite(TextureManager::GetTexture("flag"));
		sprite.setPosition(x, y);
		background.setPosition(x, y);
		foreground.setPosition(x, y);
		flag.setPosition(x, y);
		revealed = false;
		hasFlag = false;
		isDebug = false;
	}

	//setter  for the sprite
	void setSprite(sf::Sprite sprite) {
		this->sprite = sprite;
	}

	//marks the tile as having a flag
	void setFlag() {
		this->hasFlag = true;
	}

	//removes the flag from the tile
	void removeFlag() {
		this->hasFlag = false;
	}

	//checks if the tile is blank (has no mines nearby, and therefore no "main" sprite (is neither a mine nor a number)
	bool checkBlank() {
		return this->isBlank;
	}


	//draws the tile's sprites based on the current state
	void drawTile(sf::RenderWindow& window) {
		if (revealed) {
			window.draw(background);
			window.draw(sprite);
		}
		else if (isDebug && hasFlag) {
			window.draw(background);
			window.draw(foreground);
			window.draw(flag);
			window.draw(sprite);
		}
		else if (hasFlag == true) {
			window.draw(background);
			window.draw(sprite);
			window.draw(foreground);
			window.draw(flag);
		}
		else if (isDebug == true) {
			window.draw(background);
			window.draw(foreground);
			window.draw(sprite);
		}
		else {
			window.draw(background);
			window.draw(sprite);
			window.draw(foreground);

		}
	}

	//getter for the "main" sprite
	sf::Sprite getSprite() {
		return sprite;
	}

	//gets the x coordinate
	int getX() {
		return this->x;
	}

	//gets the y coordinate
	int getY() {
		return this->y;
	}

	//reveals the tile (what happens when you click on it)
	void reveal() {
		if (!this->hasFlag) {
			this->revealed = true;
		}
	}

	//sets the debug status
	void setDebug() {
		this->isDebug = true;
	}

	//removes the debug status
	void removeDebug() {
		this->isDebug = false;
	}

	//makes the tile into a mine
	void makeMine() {
		isMine = true;
		sprite = sf::Sprite(TextureManager::GetTexture("mine"));
		sprite.setPosition(x, y);
	}

	//sets the number of the tile based on the states of it's neighbors.
	void makeNumber() {
		if (!(this->checkMine())) {
			int mineCounter = 0;
			for (int i = 0; i < adjacentTiles.size(); i++) {
				if (adjacentTiles.at(i)->checkMine()) {
					mineCounter++;
				}
			}
			switch (mineCounter) {
			case 1:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_1")));
				break;
			case 2:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_2")));
				break;
			case 3:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_3")));
				break;
			case 4:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_4")));
				break;
			case 5:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_5")));
				break;
			case 6:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_6")));
				break;
			case 7:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_7")));
				break;
			case 8:
				this->setSprite(sf::Sprite(TextureManager::GetTexture("number_8")));
				break;
			case 0:
				this->isBlank = true;
				break;
			}
			this->sprite.setPosition(x, y);
		}
	}

	//checks if the tile is a mine
	bool checkMine() {
		return isMine;
	}

	//checks if the tile is revealed
	bool isRevealed() {
		return revealed;
	}

	//checks if the tile is a flag
	bool checkFlag() {
		return hasFlag;
	}

	//adds an adjacent tile to the list of the tile's neighbors
	void addAdjacent(Tile* tile) {
		adjacentTiles.push_back(tile);
	}

	//returns the number of neighbors that a tile has.
	void numOfNeighbours() {
		cout << adjacentTiles.size();
	}


};


//BOARD CLASS

class Board {
private:
	//state of a board, including the 2D vector of tiles, board specifics, and various miscellaneous sprites.
	vector<vector<Tile>> tiles;
	int boardLength;
	int boardHeight;
	int numMines;
	int minesLeft;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite test3;
	sf::Sprite debug;
	sf::Sprite smile;
	sf::Sprite int1;
	sf::Sprite int2;
	sf::Sprite int3;
	sf::Sprite negative;
	bool lock = false;
	bool booldebug = false;
	int flagsLeft;
	bool negativeCounter = false;


public:
	//constructor for the board
	Board(unsigned int length, unsigned int height, unsigned int numMines) {
		this->boardLength = length;
		this->boardHeight = height;
		this->numMines = numMines;
		this->minesLeft = numMines;
		this->flagsLeft = numMines;
		test1 = sf::Sprite(TextureManager::GetTexture("test_1"));
		test1.setPosition(((boardLength * 32) / 2 + 160), (boardHeight * 32));
		test2 = sf::Sprite(TextureManager::GetTexture("test_2"));
		test2.setPosition(((boardLength * 32) / 2 + 224), (boardHeight * 32));
		test3 = sf::Sprite(TextureManager::GetTexture("test_3"));
		test3.setPosition(((boardLength * 32) / 2 + 288), (boardHeight * 32));
		debug = sf::Sprite(TextureManager::GetTexture("debug"));
		debug.setPosition(((boardLength * 32) / 2 + 96), (boardHeight * 32));
		smile = sf::Sprite(TextureManager::GetTexture("face_happy"));
		smile.setPosition((boardLength * 32) / 2 - 32, (boardHeight * 32));
		int1 = sf::Sprite(TextureManager::GetTexture("digits"));
		int1.setPosition(21, (boardHeight * 32));
		int2 = sf::Sprite((TextureManager::GetTexture("digits")));
		int2.setPosition(42, (boardHeight * 32));
		int3 = sf::Sprite((TextureManager::GetTexture("digits")));
		int3.setPosition(63, (boardHeight * 32));
		negative = sf::Sprite((TextureManager::GetTexture("digits")));
		negative.setPosition(0, (boardHeight * 32));
		this->flagCounter();



		for (int i = 0; i < boardLength; i++) {
			vector<Tile> newVector;
			for (int j = 0; j < boardHeight; j++) {
				Tile newTile((i * 32), (j * 32), false);
				newVector.push_back(newTile);
			}
			tiles.push_back(newVector);
		}

	}

	//getter for the tiles
	vector<vector<Tile>>* returnTiles() {
		return &this->tiles;
	}

	//checks if the board is in the "locked" state (happens when a game ends)
	bool checkLock() {
		return this->lock;
	}

	//distributes the mines
	void distributeMines(int num) {
		vector<int> mineNums;
		while (mineNums.size() < numMines) {
			int newInt = Random::Int(0, (boardLength * boardHeight) - 1);
			bool contains = false;
			for (int i = 0; i < mineNums.size(); i++) {
				if (mineNums.at(i) == newInt) {
					contains = true;
				}
			}
			if (!contains) {
				mineNums.push_back(newInt);
			}
		}

		for (int i = 0; i < mineNums.size(); i++) {
			int row = mineNums.at(i) / boardLength;
			int column = mineNums.at(i) % boardLength;
			tiles.at(column).at(row).makeMine();
		}


	}

	//goes through every tile and sets neighbors
	void setAdjacents() {
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				Tile& currentTile = tiles.at(i).at(j);
				if ((i + 1) < boardLength) {
					currentTile.addAdjacent((&tiles.at(i + 1).at(j)));
				}
				if ((j + 1) < boardHeight) {
					currentTile.addAdjacent((&tiles.at(i).at(j + 1)));
				}
				if ((((j + 1) < boardHeight)) & ((i + 1) < boardLength)) {
					currentTile.addAdjacent((&tiles.at(i + 1).at(j + 1)));
				}
				if ((j - 1) >= 0) {
					currentTile.addAdjacent((&tiles.at(i).at(j - 1)));
				}
				if ((i - 1) >= 0) {
					currentTile.addAdjacent((&tiles.at(i - 1).at(j)));
				}
				if (((i - 1) >= 0) & ((j - 1) >= 0)) {
					currentTile.addAdjacent((&tiles.at(i - 1).at(j - 1)));
				}

				if (((i + 1) < boardLength) & ((j - 1) >= 0)) {
					currentTile.addAdjacent((&tiles.at(i + 1).at(j - 1)));
				}
				if (((i - 1) >= 0) & ((j + 1) < boardHeight)) {
					currentTile.addAdjacent((&tiles.at(i - 1).at(j + 1)));
				}
				tiles.at(i).at(j).makeNumber();


			}
		}
	}

	//code that runs when a flag is placed down
	void plusFlag() {
		this->flagsLeft -= 1;
	}

	//code that runs when a flag is removed
	void minusFlag() {
		this->flagsLeft += 1;
	}


	//displays the board
	void DisplayBoard(sf::RenderWindow& window) {
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				tiles.at(i).at(j).drawTile(window);
			}
		}
		window.draw(test1);
		window.draw(test2);
		window.draw(test3);
		window.draw(debug);
		window.draw(smile);
		window.draw(int1);
		window.draw(int2);
		window.draw(int3);
		if (this->negativeCounter) {
			negative.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
			window.draw(negative);
		}
	}

	//what happpens when a board is reset using the happy face (counters reset, new set of mines, new numbers, etc)
	void reset() {
		tiles.clear();
		test1 = sf::Sprite(TextureManager::GetTexture("test_1"));
		test1.setPosition(((boardLength * 32) / 2 + 160), (boardHeight * 32));
		test2 = sf::Sprite(TextureManager::GetTexture("test_2"));
		test2.setPosition(((boardLength * 32) / 2 + 224), (boardHeight * 32));
		test3 = sf::Sprite(TextureManager::GetTexture("test_3"));
		test3.setPosition(((boardLength * 32) / 2 + 288), (boardHeight * 32));
		debug = sf::Sprite(TextureManager::GetTexture("debug"));
		debug.setPosition(((boardLength * 32) / 2 + 96), (boardHeight * 32));
		smile = sf::Sprite(TextureManager::GetTexture("face_happy"));
		smile.setPosition((boardLength * 32) / 2 - 32, (boardHeight * 32));
		int1 = sf::Sprite((TextureManager::GetTexture("digits")));
		int1.setPosition(21, (boardHeight * 32));
		int2 = sf::Sprite((TextureManager::GetTexture("digits")));
		int2.setPosition(42, (boardHeight * 32));
		int3 = sf::Sprite((TextureManager::GetTexture("digits")));
		int3.setPosition(63, (boardHeight * 32));
		negative = sf::Sprite((TextureManager::GetTexture("digits")));
		negative.setPosition(0, (boardHeight * 32));
		flagsLeft = minesLeft;
		this->negativeCounter = false;
		this->flagCounter();

		for (int i = 0; i < boardLength; i++) {
			vector<Tile> newVector;
			for (int j = 0; j < boardHeight; j++) {
				Tile newTile((i * 32), (j * 32), false);
				newVector.push_back(newTile);
			}
			tiles.push_back(newVector);
		}

		lock = false;
	}

	//code for running when a game ends
	void lose() {
		this->lock = true;
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				if (tiles.at(i).at(j).checkMine()) {
					if (tiles.at(i).at(j).checkFlag()) {
						tiles.at(i).at(j).removeFlag();
						this->minusFlag();
					}
					tiles.at(i).at(j).reveal();
				}
			}
		}
		smile.setTexture(TextureManager::GetTexture("face_lose"));
	}

	//recursive algorithm for revealing all connected blank tiles when a blank tile is revealed
	void recursiveReveal(int x, int y) {
		vector<Tile*> adjacent = tiles.at(x).at(y).adjacentTiles;
		int mines = 0;
		for (int i = 0; i < adjacent.size(); i++) {
			if (adjacent.at(i)->checkMine()) {
				mines++;
			}
		}

		if (mines != 0 || tiles.at(x).at(y).isRevealed() || tiles.at(x).at(y).checkFlag()) {
			tiles.at(x).at(y).reveal();
		}

		else {
			tiles.at(x).at(y).reveal();
			for (int j = 0; j < adjacent.size(); j++) {
				recursiveReveal(adjacent.at(j)->getX() / 32, adjacent.at(j)->getY() / 32);

			}
		}

	}

	//code that runs if the player wins
	void checkWin() {
		bool didIwin = false;
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				if (!(tiles.at(i).at(j).checkMine())) {
					if (!tiles.at(i).at(j).isRevealed()) {
						return;
					}
				}
			}
			didIwin = true;
		}

		if (didIwin == true) {
			smile.setTexture(TextureManager::GetTexture("face_win"));
			for (int i = 0; i < tiles.size(); i++) {
				for (int j = 0; j < tiles.at(i).size(); j++) {
					if ((tiles.at(i).at(j).checkMine()) & (!tiles.at(i).at(j).checkFlag())) {
						tiles.at(i).at(j).setFlag();
						this->plusFlag();
					}
				}
			}
			this->lock = true;
		}
	}

	//code that runs when the debug option is clicked and it is NOT in debug state
	void debugBoard() {
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				if (tiles.at(i).at(j).checkMine()) {
					tiles.at(i).at(j).setDebug();
				}
			}
		}
	}

	//code that runs when the debug option is clicked and it IS in debug state
	void undebugBoard() {
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles.at(i).size(); j++) {
				if (tiles.at(i).at(j).checkMine()) {
					tiles.at(i).at(j).removeDebug();
				}
			}
		}
	}

	//code that switches debug states
	void debugSwitch() {
		if (booldebug) {
			this->booldebug = false;
			this->undebugBoard();
		}
		else {
			this->booldebug = true;
			this->debugBoard();
		}
	}

	//loads a borad from a .brd file
	void loadFromFile(int input) {
		ifstream inFs;
		string line;
		int minecounter = 0;
		switch (input) {
		case 1:
			inFs.open("boards/testboard1.brd");
			break;
		case 2:
			inFs.open("boards/testboard2.brd");
			break;
		case 3:
			inFs.open("boards/testboard3.brd");
			break;

		}

		int lineNum = 0;
		vector<string> lines;
		while (!inFs.fail()) {
			getline(inFs, line, '\n');
			lines.push_back(line);
		}
		lines.pop_back();

		this->reset();
		for (int i = 0; i < lines.size(); i++) {
			for (int j = 0; j < lines.at(i).length(); j++) {
				if (lines.at(i).at(j) == '1') {
					tiles.at(j).at(i).makeMine();
					minecounter++;
				}
			}
		}
		this->flagsLeft = minecounter;
		this->flagCounter();

		this->setAdjacents();

		if (booldebug) {
			debugBoard();
		}

	}

	//checks if the board is in the debug state or not
	bool isDebugging() {
		return this->booldebug;
	}

	//counts the current number of flags on a board
	void flagCounter() {
		bool isnegative = false;
		int firstint = flagsLeft / 100;
		int secondint = flagsLeft / 10;
		int thirdint = flagsLeft % 10;
		if (secondint >= 10) {
			secondint = secondint % 10;
		}


		if (secondint < 0 || thirdint < 0) {
			isnegative = true;
			secondint = secondint - secondint - secondint;
			thirdint = thirdint - thirdint - thirdint;
		}
		if (isnegative) {
			this->negativeCounter = true;
		}
		if (!isnegative) {
			this->negativeCounter = false;
		}

		int1.setTextureRect(sf::IntRect(firstint * 21, 0, 21, 32));
		int2.setTextureRect(sf::IntRect(secondint * 21, 0, 21, 32));
		int3.setTextureRect(sf::IntRect(thirdint * 21, 0, 21, 32));

	}
};
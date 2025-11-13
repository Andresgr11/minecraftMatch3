#pragma once
#include "iceBlock.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Gem;

class Board
{
private:
	Gem*** board;
	Texture gemTextures[GEM_TYPE_QUANTITY];
	iceBlock** iceBlockBoard;
	Texture iceBlockTexture;
	Texture bombTexture;
	int explotingGems(int row, int col, int dRow, int dCol, int kind);
	bool gravity();
	bool fillOnTop();
	int boardRows;
	int boardCols;
public:
	int selectedGemRow;
	int selectedGemCol;
	int totalMatches;
	int diamondsCleared;
	int goldCleared;
	int iceBlocksBroken;
	int hitCounter;
	int fiveGemMatch;
	Board();
	~Board();
	Sprite* getGem(int row, int col);
	Gem* getGemType(int row, int col) const;
	Sprite* getIceBlock(int row, int col);
	int getRows() const { return boardRows; }
	int getCols() const { return boardCols; }
	int getGemKind(int row, int col);
	bool getIsFrozen(int row, int col);
	void setLocation(int row, int col, float x, float y);
	void selectGem(int row, int col);
	bool swapping(int row1, int col1, int row2, int col2);
	bool match();
	bool hitIceAndGems();
	void deleteFadedGems();
	int processMatches();
	void bombCreation(int row, int col);
	void drawBoard(RenderWindow& window);
	void initializeBoard(int row, int col);
	void clearInitialMatches();
	bool updateBoard();
	bool fallingGems();
	bool updateAnimations(float dt);
};
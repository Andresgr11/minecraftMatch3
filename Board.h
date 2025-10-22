#pragma once
#include "Gem.h"
#include "iceBlock.h"

class Board
{
private:
	Gem board[BOARD_ROWS][BOARD_COLS];
	Texture gemTextures[GEM_TYPE_QUANTITY];
	iceBlock iceBlockBoard[BOARD_ROWS][BOARD_COLS];
	Texture iceBlockTexture;
public:
	int selectedGemRow;
	int selectedGemCol;
	int totalMatches;
	int diamondsCleared;
	int iceBlocksBroken;
	int hitCounter;
	Board();
	Sprite* getGem(int row, int col);
	Sprite* getIceBlock(int row, int col);
	int getGemKind(int row, int col);
	bool getIsFrozen(int row, int col);
	void setLocation(int row, int col, float x, float y);
	void selectGem(int row, int col);
	bool swapping(int row1, int col1, int row2, int col2);
	bool match();
	bool hitIceAndGems();
	bool removeGems();
	void initializeBoard();
	bool updateBoard();
	bool isAnimating();
};
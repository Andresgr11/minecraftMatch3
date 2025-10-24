#pragma once
#include "Gem.h"
#include "iceBlock.h"
#include "normalGem.h"
#include "bombGem.h"

class Board
{
private:
	Gem* board[BOARD_ROWS][BOARD_COLS];
	Texture gemTextures[GEM_TYPE_QUANTITY];
	iceBlock iceBlockBoard[BOARD_ROWS][BOARD_COLS];
	Texture iceBlockTexture;
	Texture bombTexture;
	int explotingGems(int row, int col, int dRow, int dCol, int kind);
	bool gravity();
	bool fillOnTop();
public:
	int selectedGemRow;
	int selectedGemCol;
	int totalMatches;
	int diamondsCleared;
	int iceBlocksBroken;
	int hitCounter;
	Board();
	~Board();
	Sprite* getGem(int row, int col);
	Gem* getGemType(int row, int col);
	Sprite* getIceBlock(int row, int col);
	int getGemKind(int row, int col);
	bool getIsFrozen(int row, int col);
	void setLocation(int row, int col, float x, float y);
	void selectGem(int row, int col);
	bool swapping(int row1, int col1, int row2, int col2);
	bool match();
	bool hitIceAndGems();
	int removeGems();
	void deleteFadedGems();
	bool bombExplosion();
	void bombCreation(int row, int col);
	void initializeBoard();
	void clearInitialMatches();
	bool updateBoard();
	bool fallingGems();
	bool updateAnimations(float dt);
};
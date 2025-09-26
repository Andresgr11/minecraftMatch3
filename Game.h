#pragma once
#include "Board.h"

class Game
{
private:
	RenderWindow* window;
	Board gameBoard;
	bool menu;
	bool playing;
	bool gameOver;
	void gameMenu();
	void gamePlay();
	void endGame();
public:
	Game();
	int click;
	int points;
	int movements;
};
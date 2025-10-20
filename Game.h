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
	String missionText;
	void gameMenu();
	void gamePlay();
	void endGame();
	void missions();
public:
	Game();
	int click;
	int points;
	int movements;
};
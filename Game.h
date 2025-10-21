#pragma once
#include "Board.h"
#include "iceBlock.h"

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
	Board iceBlocks;
public:
	Game();
	~Game() { delete window; }
	int click;
	int points;
	int movements;
	int progressMission;
};
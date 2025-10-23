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
	bool levelComplete;
	bool gameOver;
	void gameMenu();
	void gamePlay();
	void gameWin();
	void endGame();
	void missions();
	void missionProgress();
	Board iceBlocks;
	String missionText;
	String progressText;
	String finalProgressText;
public:
	Game();
	~Game() { delete window; }
	int click;
	int points;
	int movements;
	int missionType;
	bool objetiveCompleted;
};
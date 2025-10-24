#pragma once
#include "Board.h"
#include "iceBlock.h"
#include "Gem.h"

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
	enum class gameState { idle, swapping, checkingSwaps, checkingMatches, fading, falling };
	gameState currentState;
	Clock gameClock;
	Vector2i selectedGem;
	Vector2i swappedGem;
	bool gemToBomb;
public:
	Game();
	~Game() { delete window; }
	int click;
	int points;
	int movements;
	int missionType;
	bool objetiveCompleted;
};
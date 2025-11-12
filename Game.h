#pragma once
#include <SFML/Audio.hpp>
#include "Board.h"
#include "iceBlock.h"
#include "Gem.h"


class Game
{
private:
	RenderWindow* window;
	Board gameBoard;
	Music backgroundMusic[4];
	SoundBuffer audioBuffers[5];
	Sound* iceBreak;
	Sound* tntExplosion;
	Sound* missionCompleteSound;
	Sound* levelCompleteSound;
	Sound* death;
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
	string missionText;
	string progressText;
	string finalProgressText;
	enum class gameState { idle, swapping, checkingSwaps, checkingMatches, fading, falling };
	gameState currentState;
	Clock gameClock;
	Vector2i selectedGem;
	Vector2i swappedGem;
	bool gemToBomb;
public:
	Game();
	~Game() {
		delete window; delete death; delete iceBreak; delete tntExplosion; delete missionCompleteSound; delete levelCompleteSound;
	}
	int click;
	int points;
	int movements;
	int missionType;
	int musicType;
	bool objetiveCompleted;
};
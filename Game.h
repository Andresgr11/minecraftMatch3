#pragma once
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include "Board.h"
#include "iceBlock.h"
#include "Gem.h"
#include "playersList.h"
#include "player.h"

class Game
{
private:
	RenderWindow* window;
	Board gameBoard;
	Texture backgroundTexture;
	Sprite* backgroundSprite;
	Music backgroundMusic[4];
	SoundBuffer audioBuffers[5];
	Sound* iceBreak;
	Sound* tntExplosion;
	Sound* missionCompleteSound;
	Sound* levelCompleteSound;
	Sound* death;
	Font font;
	bool menu;
	bool levelSelect;
	bool playing;
	bool levelComplete;
	bool gameOver;
	void gameMenu();
	void levelSelection();
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
	playersList players;
	node<player>* currentPlayerNode;
	string currentPlayerName;        
	const string PLAYERS_FILE = "assets\\players_data.txt";
public:
	Game();
	~Game() { delete window; delete backgroundSprite; delete death; delete iceBreak; delete tntExplosion; delete missionCompleteSound; delete levelCompleteSound; }
	int click;
	int points;
	int movements;
	int missionType;
	int musicType;
	bool objetiveCompleted;
};
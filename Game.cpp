#include "Game.h"

void Game::gameMenu()
{
	while (window->isOpen() && menu)
	{
		Font font("assets\\Minecraft.otf");

		Text title(font, "Minecraft Match-3");
		title.setCharacterSize(40);
		title.setFillColor(Color::White);
		title.setPosition(Vector2f{ 40, 30 });

		RectangleShape startButton(Vector2f(200, 50));
		startButton.setFillColor(Color::Green);
		startButton.setPosition(Vector2f{ 300, 300 });

		Text startText(font, "Iniciar");
		startText.setCharacterSize(30);
		startText.setFillColor(Color::White);
		startText.setPosition(Vector2f{ 350, 305 });

		Texture backgroundTexture;
		backgroundTexture.loadFromFile("assets\\bg.png");
		Sprite backgroundSprite(backgroundTexture);
		backgroundSprite.setPosition(Vector2f{ 0, 0 });

		window->draw(backgroundSprite);
		window->draw(title);
		window->draw(startButton);
		window->draw(startText);

		Vector2i pos = Mouse::getPosition(*window);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
				window->close();

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (startButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton Iniciar presionado" << endl;
						playing = true;
						menu = false;
						break;
					}

				}
			}
		}
		window->display();
	}
}

void Game::gamePlay()
{
	gameBoard.initializeBoard();
	bool oPlaying = playing;
	playing = false;
	gameBoard.clearInitialMatches();
	playing = oPlaying;

	gameBoard.diamondsCleared = 0;
	gameBoard.iceBlocksBroken = 0;

	missions();

	currentState = gameState::idle;
	gameClock.restart();

	while (window->isOpen() && playing)
	{
		float dt = gameClock.restart().asSeconds();
		bool isAnimating = gameBoard.updateAnimations(dt);
		bool stateChanging = false;

		missionProgress();

		Font font("assets\\Minecraft.otf");

		String pointsText = "Puntos: " + to_string(points);
		String movesText = "Movimientos: " + to_string(movements);
		String objetive = "Objetivo: " + missionText;
		String progress = "Progreso: " + progressText;

		Text score(font, pointsText);
		score.setCharacterSize(30);
		score.setFillColor(Color::White);
		score.setPosition(Vector2f{ 500, 20 });

		Text moves(font, movesText);
		moves.setCharacterSize(30);
		moves.setFillColor(Color::White);
		moves.setPosition(Vector2f{ 500, 60 });

		Text title(font, "Minecraft Match-3");
		title.setCharacterSize(40);
		title.setFillColor(Color::White);
		title.setPosition(Vector2f{ 40, 30 });

		Text objetiveText(font, objetive);
		objetiveText.setCharacterSize(20);
		objetiveText.setFillColor(Color::White);
		objetiveText.setPosition(Vector2f{ 50, 100 });

		Text progressText(font, progress);
		progressText.setCharacterSize(20);
		progressText.setFillColor(Color::White);
		progressText.setPosition(Vector2f{ 50, 130 });

		RectangleShape exitButton(Vector2f(200, 50));
		exitButton.setFillColor(Color::Yellow);
		exitButton.setPosition(Vector2f{ 50, 400 });

		Text exitText(font, "Salir");
		exitText.setCharacterSize(30);
		exitText.setFillColor(Color::White);
		exitText.setPosition(Vector2f{ 115, 405 });

		Texture backgroundTexture;
		backgroundTexture.loadFromFile("assets\\bg.png");
		Sprite backgroundSprite(backgroundTexture);
		backgroundSprite.setPosition(Vector2f{ 0, 0 });

		window->draw(backgroundSprite);
		window->draw(title);
		window->draw(score);
		window->draw(moves);
		window->draw(objetiveText);
		window->draw(progressText);
		window->draw(exitButton);
		window->draw(exitText);

		gameBoard.drawBoard(*window);

		Vector2i pos = Mouse::getPosition(*window);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
				window->close();

			if (currentState == gameState::idle && !isAnimating)
			{
				if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
				{
					if (mouseButtonPressed->button == Mouse::Button::Left)
					{
						bool gemClicked = false;
						Vector2i selectingGem;

						for (int i = 0; i < BOARD_ROWS; i++)
						{
							for (int j = 0; j < BOARD_COLS; j++)
							{
								if (gameBoard.getGem(i, j)->getGlobalBounds().contains(Vector2f(pos)))
								{
									gemClicked = true;
									selectingGem = Vector2i(i, j);
									break;									
								}
							}
							if (gemClicked)	break;
						}

						if (gemClicked)
						{
							click++;
							cout << "Click count: " << click << endl;

							if (click == 1)
							{
								gameBoard.selectGem(selectingGem.x, selectingGem.y);
								selectedGem = selectingGem;
							}
							else if (click == 2) // <<< USAR "ELSE IF"
							{
								swappedGem = selectingGem;

								if (selectedGem == swappedGem)
								{
									gameBoard.getGemType(selectedGem.x, selectedGem.y)->getSprite()->setColor(Color::White);
								}
								else if (gameBoard.swapping(selectedGem.x, selectedGem.y, swappedGem.x, swappedGem.y))
								{
									movements--;
									currentState = gameState::swapping;
									stateChanging = true;
								}
								else
								{
									currentState = gameState::idle;
								}
								click = 0;
							}

						}



						if (exitButton.getGlobalBounds().contains(Vector2f(pos)))
						{
							cout << "Boton Salir presionado" << endl;
							playing = false;
							window->close();
							break;
						}
					}
				}
			}
		}
		if (!isAnimating && !stateChanging)
		{
			switch (currentState)
			{
			case gameState::idle:
				break;

			case gameState::swapping:
			{
				bool bombSwap = (gameBoard.getGemType(selectedGem.x, selectedGem.y)->getType() == Gem::GemType::Bomb || gameBoard.getGemType(swappedGem.x, swappedGem.y)->getType() == Gem::GemType::Bomb);
				if (bombSwap)
				{
					gameBoard.getGemType(selectedGem.x, selectedGem.y)->mark();
					gameBoard.getGemType(swappedGem.x, swappedGem.y)->mark();
				}
				if (gameBoard.match() || bombSwap)
				{
					currentState = gameState::checkingMatches;
					gemToBomb = true;
				}
				else
				{
					cout << "Match no encontrado. Deshaciendo." << endl;
					gameBoard.swapping(selectedGem.x, selectedGem.y, swappedGem.x, swappedGem.y);
					movements++;
					currentState = gameState::checkingSwaps;
				}
				click = 0;
			}
			break;

			case gameState::checkingSwaps:
			{
				currentState = gameState::idle;
				break;
			}
			case gameState::checkingMatches:
			{
				gameBoard.hitIceAndGems();

				if (gemToBomb)
				{
					if (gameBoard.getGemType(selectedGem.x, selectedGem.y) != nullptr && gameBoard.getGemType(selectedGem.x, selectedGem.y)->isMarked())
					{
						gameBoard.bombCreation(selectedGem.x, selectedGem.y);
					}
					if (gameBoard.getGemType(swappedGem.x, swappedGem.y) != nullptr && gameBoard.getGemType(swappedGem.x, swappedGem.y)->isMarked())
					{
						gameBoard.bombCreation(swappedGem.x, swappedGem.y);
					}
					gemToBomb = false;
				}

				for (int i = 0; i < BOARD_ROWS; i++)
				{
					for (int j = 0; j < BOARD_COLS; j++)
					{
						if (gameBoard.getGemType(i, j) != nullptr && gameBoard.getGemType(i, j)->isMarked())
						{
							gameBoard.bombCreation(i, j);
						}
					}
				}

				int matches = gameBoard.processMatches();
				points += matches * 10;
				missionProgress();

				if (matches > 0)
				{
					currentState = gameState::fading;
				}
				else if (gameBoard.match())
				{
					currentState = gameState::checkingMatches;
				}
				else
				{
					currentState = gameState::idle;

					if (movements == 0)
					{
						if (objetiveCompleted)
						{
							levelComplete = true;
							playing = false;
						}
						else
						{
							gameOver = true;
							playing = false;
						}
					}
				}
				break;
			}
			case gameState::fading:
			{
				gameBoard.deleteFadedGems();
				gameBoard.fallingGems();
				currentState = gameState::falling;
				break;
			}
			case gameState::falling:
			{
				bool fallGems = gameBoard.fallingGems();

				if (fallGems)
				{
					currentState = gameState::falling;
				}
				else
				{
					if (gameBoard.match())
					{
						currentState = gameState::checkingMatches;
					}
					else
					{
						currentState = gameState::idle;

						if (movements == 0)
						{
							if (objetiveCompleted)
							{
								levelComplete = true;
								playing = false;
							}
							else
							{
								gameOver = true;
								playing = false;
							}
						}
					}
				}
				break;
			}
			}
		}
		window->display();
	}
}

void Game::gameWin()
{
	while (window->isOpen() && levelComplete)
	{
		Font font("assets\\Minecraft.otf");
		
		Text winningText(font, "¡Has ganado!");
		winningText.setCharacterSize(50);
		winningText.setFillColor(Color::White);
		winningText.setPosition(Vector2f{ 50, 30 });

		String finalPointsText = "Puntuacion: " + to_string(points);
		String movesText = "Movimientos: " + to_string(movements);
		String objetive = "Objetivo: " + missionText;
		String progress = "Progreso: " + progressText;
		String finalProgress = finalProgressText;

		Text finalScore(font, finalPointsText);
		finalScore.setCharacterSize(30);
		finalScore.setFillColor(Color::White);
		finalScore.setPosition(Vector2f{ 500, 20 });

		Text moves(font, movesText);
		moves.setCharacterSize(30);
		moves.setFillColor(Color::White);
		moves.setPosition(Vector2f{ 500, 60 });

		Text objetiveText(font, objetive);
		objetiveText.setCharacterSize(20);
		objetiveText.setFillColor(Color::White);
		objetiveText.setPosition(Vector2f{ 50, 100 });

		Text progressText(font, progress);
		progressText.setCharacterSize(20);
		progressText.setFillColor(Color::White);
		progressText.setPosition(Vector2f{ 50, 130 });

		Text finalProgressMission(font, finalProgress);
		finalProgressMission.setCharacterSize(20);
		finalProgressMission.setFillColor(Color::White);
		finalProgressMission.setPosition(Vector2f{ 50, 160 });

		RectangleShape nextLevelButton(Vector2f(200, 50));
		nextLevelButton.setFillColor(Color::Blue);
		nextLevelButton.setPosition(Vector2f{ 50, 200 });

		Text nextLevelText(font, "Siguiente nivel");
		nextLevelText.setCharacterSize(25);
		nextLevelText.setFillColor(Color::White);
		nextLevelText.setPosition(Vector2f{ 60, 205 });

		RectangleShape exitButton(Vector2f(200, 50));
		exitButton.setFillColor(Color::Yellow);
		exitButton.setPosition(Vector2f{ 50, 400 });

		Text exitText(font, "Salir");
		exitText.setCharacterSize(30);
		exitText.setFillColor(Color::White);
		exitText.setPosition(Vector2f{ 115, 405 });

		window->clear(Color::Green);
		window->draw(winningText);
		window->draw(finalScore);
		window->draw(moves);
		window->draw(objetiveText);
		window->draw(progressText);
		window->draw(finalProgressMission);
		window->draw(nextLevelButton);
		window->draw(nextLevelText);
		window->draw(exitButton);
		window->draw(exitText);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
				window->close();

			Vector2i pos = Mouse::getPosition(*window);
			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (nextLevelButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton de siguiente nivel presionado" << endl;
						points = 0;
						movements = 20;
						gameBoard.diamondsCleared = 0;
						gameBoard.iceBlocksBroken = 0;
						missionType++;
						playing = true;
						levelComplete = false;
						break;
					}
					if (exitButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton Salir presionado" << endl;
						levelComplete = false;
						window->close();
						break;
					}
				}
			}
		}
		window->display();
	}
}

void Game::endGame()
{
	while (window->isOpen() && gameOver)
	{
		Font font("assets\\Minecraft.otf");

		Text gameOverText(font, "¡Has muerto!");
		gameOverText.setCharacterSize(50);
		gameOverText.setFillColor(Color::White);
		gameOverText.setPosition(Vector2f{ 50, 30 });

		String finalPointsText = "Puntuacion: " + to_string(points);
		String movesText = "Movimientos: " + to_string(movements);
		String objetive = "Objetivo: " + missionText;
		String progress = "Progreso: " + progressText;
		String finalProgress = finalProgressText;

		Text finalScore(font, finalPointsText);
		finalScore.setCharacterSize(30);
		finalScore.setFillColor(Color::White);
		finalScore.setPosition(Vector2f{ 500, 20 });

		Text moves(font, movesText);
		moves.setCharacterSize(30);
		moves.setFillColor(Color::White);
		moves.setPosition(Vector2f{ 500, 60 });

		Text objetiveText(font, objetive);
		objetiveText.setCharacterSize(20);
		objetiveText.setFillColor(Color::White);
		objetiveText.setPosition(Vector2f{ 50, 100 });

		Text progressText(font, progress);
		progressText.setCharacterSize(20);
		progressText.setFillColor(Color::White);
		progressText.setPosition(Vector2f{ 50, 130 });

		Text finalProgressMission(font, finalProgress);
		finalProgressMission.setCharacterSize(20);
		finalProgressMission.setFillColor(Color::White);
		finalProgressMission.setPosition(Vector2f{ 50, 160 });

		RectangleShape restartButton(Vector2f(200, 50));
		restartButton.setFillColor(Color::Blue);
		restartButton.setPosition(Vector2f{ 50, 200 });

		Text restartText(font, "Reaparecer");
		restartText.setCharacterSize(30);
		restartText.setFillColor(Color::White);
		restartText.setPosition(Vector2f{ 60, 205 });

		RectangleShape exitButton(Vector2f(200, 50));
		exitButton.setFillColor(Color::Yellow);
		exitButton.setPosition(Vector2f{ 50, 400 });

		Text exitText(font, "Salir");
		exitText.setCharacterSize(30);
		exitText.setFillColor(Color::White);
		exitText.setPosition(Vector2f{ 115, 405 });

		window->clear(Color::Red);
		window->draw(gameOverText);
		window->draw(finalScore);
		window->draw(moves);
		window->draw(objetiveText);
		window->draw(progressText);
		window->draw(finalProgressMission);
		window->draw(restartButton);
		window->draw(restartText);
		window->draw(exitButton);
		window->draw(exitText);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
				window->close();

			Vector2i pos = Mouse::getPosition(*window);

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (restartButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton Reiniciar presionado" << endl;
						points = 0;
						movements = 20;
						gameBoard.diamondsCleared = 0;
						gameBoard.iceBlocksBroken = 0;
						playing = true;
						gameOver = false;
						break;
					}
					if (exitButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton Salir presionado" << endl;
						gameOver = false;
						window->close();
						break;
					}
				}
			}
		}
		window->display();
	}
}

void Game::missions()
{
	objetiveCompleted = false;
	if (missionType == 0)
	{
		missionText = "Rompe 2 bloques de hielo.";
	}
	else if (missionType == 1)
	{
		missionText = "Obten 1500 puntos.";
	}
	else if (missionType == 2)
	{
		missionText = "Elimina 30 diamantes.";
	}
	else if (missionType > 2)
	{
		missionType = 0;
	}
}

void Game::missionProgress()
{
	if (missionType == 0)
	{		
		progressText = to_string(gameBoard.iceBlocksBroken) + "/2";

		if (gameBoard.iceBlocksBroken >= 2)
		{
			objetiveCompleted = true;
			finalProgressText = "Objetivo cumplido.";
			movements = 0;		
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
	else if (missionType == 1)
	{
		progressText = to_string(points) + "/1500";

		if (points >= 1500)
		{
			objetiveCompleted = true;
			finalProgressText = "Objetivo cumplido.";
			movements = 0;		
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
	else if (missionType == 2)
	{
		progressText = to_string(gameBoard.diamondsCleared) + "/30";

		if (gameBoard.diamondsCleared >= 30)
		{
			objetiveCompleted = true;
			finalProgressText = "Objetivo cumplido.";
			movements = 0;
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
}

Game::Game()
{
	playing = false;
	levelComplete = false;
	gameOver = false;
	objetiveCompleted = false;
	menu = true;
	window = new RenderWindow(VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), WINDOW_TITLE);
	window->setFramerateLimit(FPS);

	click = 0, points = 0;
	movements = 20;
	missionType = 0;
	
	currentState = gameState::idle;
	selectedGem = Vector2i(-1, -1);
	swappedGem = Vector2i(-1, -1);
	gemToBomb = false;

	gameMenu();
	while (playing || levelComplete || gameOver)
	{
		if (playing)
		{
			gamePlay();

		}
		else if (levelComplete)
		{
			gameWin();
		}
		else if (gameOver)
		{
			endGame();
		}
	}
}
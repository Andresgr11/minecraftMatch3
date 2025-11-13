#include "Game.h"

Game::Game()
{
	srand(static_cast<unsigned int>(time(0)));

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
	musicType = rand() % 4;

	if (!backgroundTexture.loadFromFile("assets\\bg.png"))
	{
		cerr << "Error al cargar la textura de fondo." << endl;
	}
	backgroundSprite = new Sprite(backgroundTexture);

	if (!font.openFromFile("assets\\Minecraft.otf"))
	{
		cerr << "Error al cargar la fuente de texto." << endl;
	}

	if (!backgroundMusic[0].openFromFile("assets\\sweden.mp3"))
	{
		cerr << "Error al cargar la musica de fondo Sweden." << endl;
	}
	if (!backgroundMusic[1].openFromFile("assets\\minecraft.mp3"))
	{
		cerr << "Error al cargar la musica de fondo Minecraft." << endl;
	}
	if (!backgroundMusic[2].openFromFile("assets\\haggstrom.mp3"))
	{
		cerr << "Error al cargar la musica de fondo Haggstrom." << endl;
	}
	if (!backgroundMusic[3].openFromFile("assets\\wethands.mp3"))
	{
		cerr << "Error al cargar la musica de fondo Wet Hands." << endl;
	}
	
	if (!audioBuffers[0].loadFromFile("assets\\icebreak.mp3"))
	{
		cerr << "Error al cargar el sonido Ice Break." << endl;
	}
	if (!audioBuffers[1].loadFromFile("assets\\tntexplosion.mp3"))
	{
		cerr << "Error al cargar el sonido TNT Explosion." << endl;
	}
	if (!audioBuffers[2].loadFromFile("assets\\endermandeath.mp3"))
	{
		cerr << "Error al cargar el sonido Enderman Death." << endl;
	}
	if (!audioBuffers[3].loadFromFile("assets\\missioncomplete.mp3"))
	{
		cerr << "Error al cargar el sonido Mission Complete." << endl;
	}
	if (!audioBuffers[4].loadFromFile("assets\\levelcomplete.mp3"))
	{
		cerr << "Error al cargar el sonido Level Complete." << endl;
	}

	iceBreak = new Sound(audioBuffers[0]);
	tntExplosion = new Sound(audioBuffers[1]);
	death = new Sound(audioBuffers[2]);
	missionCompleteSound = new Sound(audioBuffers[3]);
	levelCompleteSound = new Sound(audioBuffers[4]);

	currentState = gameState::idle;
	selectedGem = Vector2i(-1, -1);
	swappedGem = Vector2i(-1, -1);
	gemToBomb = false;

	currentPlayerNode = nullptr;
	players.loadFromFile(PLAYERS_FILE);

	gameMenu();
	while (playing || levelSelect || levelComplete || gameOver)
	{
		if (playing)
		{
			gamePlay();
		}
		else if (levelSelect)
		{
			levelSelection();
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

void Game::gameMenu()
{
	backgroundMusic[musicType].play();
	backgroundMusic[musicType].setLooping(true);

	players.sortPlayersByScore();

	string topNames[3];
	int topScores[3];
	players.topPlayers(3, topNames, topScores);

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

	Text rankTitle(font, "TOP 3 JUGADORES");
	rankTitle.setCharacterSize(30);
	rankTitle.setFillColor(Color::Yellow);
	rankTitle.setPosition(Vector2f{ 510, 30 });

	Text rankTexts[3] = {
		Text(font, ""), Text(font, ""), Text(font, "")
	};

	for (int i = 0; i < 3; i++)
	{
		rankTexts[i].setFont(font);
		rankTexts[i].setCharacterSize(25);
		rankTexts[i].setFillColor(Color::White);
		rankTexts[i].setPosition(Vector2f{ 510.f, 70.f + (i * 35.f) });
		string rankString = to_string(i + 1) + ". " + topNames[i] + " - " + to_string(topScores[i]);
		rankTexts[i].setString(rankString);
	}

	string playerNameInput = "";
	Text nameInput(font, "Ingresa tu nombre:");
	nameInput.setCharacterSize(30);
	nameInput.setFillColor(Color::White);
	nameInput.setPosition(Vector2f{ 250, 200 });

	RectangleShape nameBox(Vector2f(300, 50));
	nameBox.setFillColor(Color::White);
	nameBox.setOutlineColor(Color::Black);
	nameBox.setOutlineThickness(2);
	nameBox.setPosition(Vector2f{ 250, 240 });

	Text nameText(font, "");
	nameText.setCharacterSize(30);
	nameText.setFillColor(Color::Black);
	nameText.setPosition(Vector2f{ 255, 245 });

	backgroundSprite->setPosition(Vector2f{ 0, 0 });

	while (window->isOpen() && menu)
	{
		window->draw(*backgroundSprite);
		window->draw(title);
		window->draw(nameInput);
		window->draw(nameBox);
		window->draw(nameText);
		window->draw(startButton);
		window->draw(startText);
		window->draw(rankTitle);
		for (int i = 0; i < 3; i++)
		{
			window->draw(rankTexts[i]);
		}

		Vector2i pos = Mouse::getPosition(*window);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
			{
				window->close();
				menu = false;
			}

			if (const auto* textEntered = event->getIf<Event::TextEntered>())
			{
				if (textEntered->unicode >= 32 && textEntered->unicode < 128 && playerNameInput.length() < 15) {
					playerNameInput += static_cast<char>(textEntered->unicode);
					nameText.setString(playerNameInput);
				}
			}
			if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
			{
				if (keyPressed->code == Keyboard::Key::Backspace && !playerNameInput.empty()) {
					playerNameInput.pop_back();
					nameText.setString(playerNameInput);
				}
			}

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (startButton.getGlobalBounds().contains(Vector2f(pos)) && !playerNameInput.empty())
					{
						cout << "Boton Iniciar presionado" << endl;
						currentPlayerName = playerNameInput;
						currentPlayerNode = players.findOrCreatePlayer(currentPlayerName);
						levelSelect = true;
						menu = false;
						break;
					}

				}
			}
		}
		window->display();
	}
}

void Game::levelSelection()
{
	int levelsCompleted = currentPlayerNode->getData().getLevelsCompleted();

	while (window->isOpen() && levelSelect)
	{
		Text welcomeText(font, "Bienvenido, " + currentPlayerName + "!");
		welcomeText.setCharacterSize(30);
		welcomeText.setFillColor(Color::White);
		welcomeText.setPosition(Vector2f{ 30, 30 });

		Text title(font, "Selecciona un nivel");
		title.setCharacterSize(30);
		title.setFillColor(Color::White);
		title.setPosition(Vector2f{ 250, 80 });

		RectangleShape levelButtons[5];

		levelButtons[0] = RectangleShape(Vector2f(150, 50));
		levelButtons[0].setFillColor(Color::Green);
		levelButtons[0].setPosition(Vector2f{ 100, 150 });
		Text level1Text(font, "Nivel 1");
		level1Text.setCharacterSize(30);
		level1Text.setFillColor(Color::White);
		level1Text.setPosition(Vector2f{ 125, 155 });

		levelButtons[1] = RectangleShape(Vector2f(150, 50));
		levelButtons[1].setFillColor(Color::Green);
		levelButtons[1].setPosition(Vector2f{ 300, 150 });
		Text level2Text(font, "Nivel 2");
		level2Text.setCharacterSize(30);
		level2Text.setFillColor(Color::White);
		level2Text.setPosition(Vector2f{ 325, 155 });

		levelButtons[2] = RectangleShape(Vector2f(150, 50));
		levelButtons[2].setFillColor(Color::Green);
		levelButtons[2].setPosition(Vector2f{ 500, 150 });
		Text level3Text(font, "Nivel 3");
		level3Text.setCharacterSize(30);
		level3Text.setFillColor(Color::White);
		level3Text.setPosition(Vector2f{ 525, 155 });

		levelButtons[3] = RectangleShape(Vector2f(150, 50));
		levelButtons[3].setFillColor(Color::Green);
		levelButtons[3].setPosition(Vector2f{ 200, 250 });
		Text level4Text(font, "Nivel 4");
		level4Text.setCharacterSize(30);
		level4Text.setFillColor(Color::White);
		level4Text.setPosition(Vector2f{ 225, 255 });

		levelButtons[4] = RectangleShape(Vector2f(150, 50));
		levelButtons[4].setFillColor(Color::Green);
		levelButtons[4].setPosition(Vector2f{ 400, 250 });
		Text level5Text(font, "Nivel 5");
		level5Text.setCharacterSize(30);
		level5Text.setFillColor(Color::White);
		level5Text.setPosition(Vector2f{ 425, 255 });

		for (int i = 0; i < 5; i++)
		{
			if (i > levelsCompleted)
			{
				levelButtons[i].setFillColor(Color(128, 128, 128));
			}
		}

		backgroundSprite->setPosition(Vector2f{ 0, 0 });

		window->draw(*backgroundSprite);
		window->draw(welcomeText);
		window->draw(title);
		for (int i = 0; i < 5; i++)
		{
			window->draw(levelButtons[i]);
		}
		window->draw(level1Text);
		window->draw(level2Text);
		window->draw(level3Text);
		window->draw(level4Text);
		window->draw(level5Text);

		

		Vector2i pos = Mouse::getPosition(*window);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
			{
				window->close();
				levelSelect = false;
			}

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					for (int i = 0; i < 5; i++)
					{
						if (levelButtons[i].getGlobalBounds().contains(Vector2f(pos)) && i <= levelsCompleted)
						{
							cout << "Nivel " << (i + 1) << " seleccionado" << endl;
							missionType = i;
							backgroundMusic[musicType].stop();
							levelSelect = false;
							playing = true;
							break;
						}
					}
				}
			}
		}
		window->display();
	}
}

void Game::gamePlay()
{
	if (missionType > 4)
	{
		missionType = 0;
	}

	int levelRows;
	int levelCols;

	switch (missionType)
	{
	case 0:
		levelRows = 8;
		levelCols = 8;
		break;
	case 1:
		levelRows = 10;
		levelCols = 10;
		break;
	case 2:
		levelRows = 9;
		levelCols = 8;
		break;
	case 3:
		levelRows = 10;
		levelCols = 9;
		break;
	case 4:
		levelRows = 8;
		levelCols = 10;
		break;
	default:
		levelRows = 8;
		levelCols = 8;
		break;
	}
	gameBoard.initializeBoard(levelRows, levelCols);

	bool oPlaying = playing;
	playing = false;
	gameBoard.clearInitialMatches();
	playing = oPlaying;

	points = 0;
	movements = 20;

	gameBoard.diamondsCleared = 0;
	gameBoard.goldCleared = 0;
	gameBoard.iceBlocksBroken = 0;
	gameBoard.fiveGemMatch = 0;

	missions();

	currentState = gameState::idle;
	gameClock.restart();

	musicType = rand() % 4;
	backgroundMusic[musicType].play();
	backgroundMusic[musicType].setLooping(true);

	while (window->isOpen() && playing)
	{
		float dt = gameClock.restart().asSeconds();
		bool isAnimating = gameBoard.updateAnimations(dt);
		bool stateChanging = false;

		missionProgress();

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

		backgroundSprite->setPosition(Vector2f{ 0, 0 });

		window->draw(*backgroundSprite);
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
			{
				window->close();
				playing = false;
			}

			if (currentState == gameState::idle && !isAnimating)
			{
				if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
				{
					if (mouseButtonPressed->button == Mouse::Button::Left)
					{
						bool gemClicked = false;
						Vector2i selectingGem;

						for (int i = 0; i < levelRows; i++)
						{
							for (int j = 0; j < levelCols; j++)
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
							else if (click == 2)
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
				missionCompleteSound->play();
				if (bombSwap)
				{
					gameBoard.getGemType(selectedGem.x, selectedGem.y)->mark();
					gameBoard.getGemType(swappedGem.x, swappedGem.y)->mark();
					tntExplosion->play();
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
				if (gameBoard.hitIceAndGems())
				{
					iceBreak->play();
				}
				if (gemToBomb)
				{
					
					gemToBomb = false;
				}

				for (int i = 0; i < levelRows; i++)
				{
					for (int j = 0; j < levelCols; j++)
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
					missionCompleteSound->play();
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
						missionCompleteSound->play();
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
								death->play();
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
			{
				window->close();
				levelComplete = false;
			}
				
			Vector2i pos = Mouse::getPosition(*window);
			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (nextLevelButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton de siguiente nivel presionado" << endl;
						if (currentPlayerNode != nullptr)
						{
							player currentData = currentPlayerNode->getData();
							int newLevelsCompleted = missionType + 1;
							int newMaxScore = (currentData.getMaxScore() > points) ? currentData.getMaxScore() : points;
							currentPlayerNode->setData(player(currentPlayerName, newMaxScore, newLevelsCompleted));
							players.saveToFile(PLAYERS_FILE);
							missionType++;
							backgroundMusic[musicType].stop();
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
		}
		window->display();
	}
}

void Game::endGame()
{
	while (window->isOpen() && gameOver)
	{
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
			{
				window->close();
				gameOver = false;
			}
			
			Vector2i pos = Mouse::getPosition(*window);

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					if (restartButton.getGlobalBounds().contains(Vector2f(pos)))
					{
						cout << "Boton Reiniciar presionado" << endl;
						if (currentPlayerNode != nullptr)
						{
							player currentData = currentPlayerNode->getData();
							if (points > currentData.getMaxScore())
							{
								currentPlayerNode->setData(player(currentPlayerName, points, currentData.getLevelsCompleted()));
								players.saveToFile(PLAYERS_FILE);
								cout << "Nuevo puntaje maximo guardado: " << points << endl;
							}
						}
						backgroundMusic[musicType].stop();
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
	else if (missionType == 3)
	{
		missionText = "Elimina 20 lingotes de oro y rompe 2 bloques de hielo.";
	}
	else if (missionType == 4)
	{
		missionText = "Crea 2 matches de 5 gemas.";
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
			levelCompleteSound->play();
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
			levelCompleteSound->play();
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
			levelCompleteSound->play();
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
	else if (missionType == 3)
	{
		progressText = to_string(gameBoard.goldCleared) + "/30 , " + to_string(gameBoard.iceBlocksBroken) + "/3";

		if (gameBoard.goldCleared >= 30 && gameBoard.iceBlocksBroken >= 3)
		{
			objetiveCompleted = true;
			finalProgressText = "Objetivo cumplido.";
			levelCompleteSound->play();
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
	else if (missionType == 4)
	{
		progressText = to_string(gameBoard.fiveGemMatch) + "/2";

		if (gameBoard.fiveGemMatch >= 2)
		{
			objetiveCompleted = true;
			finalProgressText = "Objetivo cumplido.";
			levelCompleteSound->play();
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
}
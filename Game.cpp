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

	missions();

	while (window->isOpen() && playing)
	{
		Font font("assets\\Minecraft.otf");

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

		for (int i = 0; i < BOARD_ROWS; i++)
		{
			for (int j = 0; j < BOARD_COLS; j++)
			{
				if (gameBoard.getGem(i, j) != nullptr)
				{
					window->draw(*gameBoard.getGem(i, j));
				}
				if (gameBoard.getIsFrozen(i,j) && gameBoard.getIceBlock(i, j) != nullptr)
				{
					window->draw(*gameBoard.getIceBlock(i, j));
				}
			}
		}

		Vector2i pos = Mouse::getPosition(*window);

		while (const optional event = window->pollEvent())
		{
			if (event->is<Event::Closed>())
				window->close();

			if (const auto* mouseButtonPressed = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == Mouse::Button::Left)
				{
					for (int i = 0; i < BOARD_ROWS; i++)
					{
						for (int j = 0; j < BOARD_COLS; j++)
						{
							if (gameBoard.getGem(i, j)->getGlobalBounds().contains(Vector2f(pos)))
							{								
								click++;
								cout << "Click count: " << click << endl;
								if (click == 1)
								{
									gameBoard.selectGem(i, j);
								}
								if (click == 2)
								{
									gameBoard.swapping(gameBoard.selectedGemRow, gameBoard.selectedGemCol, i, j);

									while (gameBoard.match())
									{
										gameBoard.hitIceAndGems();
										points += gameBoard.totalMatches * 10;
										movements--;
										missionProgress();
										do
										{
											gameBoard.updateBoard();
										} while (gameBoard.updateBoard());

									}							
									click = 0;
								}
								if (movements == 0)
								{
									gameOver = true;
									playing = false;
									break;
								}
							}
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
	srand(time(0));
	int random = rand() % 3;
	missionType = random;

	if (missionType == 0)
	{
		missionText = "Elimina 15 diamantes.";
	}
	else if (missionType == 1)
	{
		missionText = "Romper 2 bloques de hielo.";
	}
	else if (missionType == 2)
	{
		missionText = "Obtenga 1000 puntos.";
	}
}

void Game::missionProgress()
{
	if (missionType == 0)
	{		
		progressText = to_string(gameBoard.diamondsCleared) + "/15";

		if (gameBoard.diamondsCleared >= 15)
		{
			finalProgressText = "Objetivo cumplido.";
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}	
	}
	else if (missionType == 1)
	{
		progressText = to_string(gameBoard.iceBlocksBroken) + "/2";

		if (gameBoard.iceBlocksBroken >= 2)
		{
			finalProgressText = "Objetivo cumplido.";
		}
		else
		{
			finalProgressText = "Objetivo no cumplido.";
		}
	}
	else if (missionType == 2)
	{
		progressText = to_string(points) + "/1000";
		if (points >= 1000)
		{
			finalProgressText = "Objetivo cumplido.";
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
	gameOver = false;
	menu = true;
	window = new RenderWindow(VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), WINDOW_TITLE);
	window->setFramerateLimit(FPS);

	click = 0, points = 0;
	movements = 20;
	missionType = 0;
	int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

	gameMenu();
	while (playing || gameOver)
	{
		if (playing)
		{
			gamePlay();

		}
		if (gameOver)
		{
			endGame();
		}
	}
}
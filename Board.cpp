#include "Board.h"
#include "Gem.h"
#include "normalGem.h" 
#include "bombGem.h"

Board::Board()
{
	gemTextures[0].loadFromFile("assets\\01.png");
	gemTextures[1].loadFromFile("assets\\02.png");
	gemTextures[2].loadFromFile("assets\\03.png");
	gemTextures[3].loadFromFile("assets\\04.png");
	gemTextures[4].loadFromFile("assets\\05.png");
	iceBlockTexture.loadFromFile("assets\\ice.png");
	bombTexture.loadFromFile("assets\\tnt.png");

	/*
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			board[i][j] = nullptr;
		}
	}	
	*/	

	boardRows = 0;
	boardCols = 0;
	selectedGemRow = -1;
	selectedGemCol = -1;
	totalMatches = 0;
	diamondsCleared = 0;
	iceBlocksBroken = 0;
	hitCounter = 0;
}

Board::~Board()
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			delete board[i][j];
		}
	}
}

Sprite* Board::getGem(int row, int col)
{
	if (board[row][col] == nullptr)
	{
		return nullptr;
	}
	return board[row][col]->getSprite();
}

Gem* Board::getGemType(int row, int col) const
{
	if (row < 0 || row >= boardRows || col < 0 || col >= boardCols)
	{
		return nullptr;
	}
	return board[row][col];
}

Sprite* Board::getIceBlock(int row, int col)
{
	return iceBlockBoard[row][col].getSprite();
}

int Board::getGemKind(int row, int col)
{
	if (board[row][col] == nullptr)
	{
		return -1;
	}
	return board[row][col]->getGemKind();
}

bool Board::getIsFrozen(int row, int col)
{
	return iceBlockBoard[row][col].getIsFrozen();
}

void Board::setLocation(int row, int col, float x, float y)
{
	board[row][col]->setLocation(x, y);
}

void Board::selectGem(int row, int col)
{
	cout << "Gema seleccionada en (" << row << ", " << col << ") del tipo #" << board[row][col]->getGemKind() << endl;
	board[row][col]->getSprite()->setColor(Color::Blue);
	selectedGemCol = col;
	selectedGemRow = row;
}

bool Board::swapping(int row1, int col1, int row2, int col2)
{
	bool swapping = false;

	if (iceBlockBoard[row1][col1].getIsFrozen() || iceBlockBoard[row2][col2].getIsFrozen())
	{
		cout << "La gema esta congelada, no se puede intercambiar." << endl;
		board[row1][col1]->getSprite()->setColor(Color::White);
		return swapping;
	}
	if ((abs(row1 - row2) == 1 && col1 == col2) || (abs(col1 - col2) == 1 && row1 == row2))
	{
		swap(board[row1][col1], board[row2][col2]);
		cout << "Gemas intercambiadas en: (" << row1 << ", " << col1 << ") y (" << row2 << ", " << col2 << ")." << endl;
		board[row1][col1]->move(static_cast<float>(BOARD_X_START + col1 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row1 * CELL_SIDE_SIZE));
		board[row2][col2]->move(static_cast<float>(BOARD_X_START + col2 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row2 * CELL_SIDE_SIZE));
		board[row1][col1]->getSprite()->setColor(Color::White);
		board[row2][col2]->getSprite()->setColor(Color::White);
		swapping = true;	
	}
	else
	{
		board[row1][col1]->getSprite()->setColor(Color::White);
		cout << "Intercambio no realizado." << endl;
		swapping = false;
	}
	return swapping;
}

bool Board::match()
{
	bool matching = false;

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (j < boardCols - 2)
			{
				if (board[i][j + 1] == nullptr || board[i][j + 2] == nullptr || board[i][j + 1]->getType() == Gem::GemType::Bomb || board[i][j + 2]->getType() == Gem::GemType::Bomb)
				{
					continue;
				}

				bool gemsMatched = (board[i][j]->getGemKind() == board[i][j + 1]->getGemKind() && board[i][j]->getGemKind() == board[i][j + 2]->getGemKind());
				bool gemsUnfrozen = (!iceBlockBoard[i][j].getIsFrozen() && !iceBlockBoard[i][j + 1].getIsFrozen() && !iceBlockBoard[i][j + 2].getIsFrozen());
				bool matchFounded = false;

				if (gemsMatched && gemsUnfrozen)
				{
					board[i][j]->mark();
					board[i][j + 1]->mark();
					board[i][j + 2]->mark();
					matching = true;
					cout << "Match encontrado en (" << i << ", " << j << "), (" << i << ", " << j + 1 << "), (" << i << ", " << j + 2 << ")" << endl;
				}
			}

			if (i < boardRows - 2)
			{
				if (board[i + 1][j] == nullptr || board[i + 2][j] == nullptr || board[i + 1][j]->getType() == Gem::GemType::Bomb || board[i + 2][j]->getType() == Gem::GemType::Bomb)
				{
					continue;
				}

				bool gemsMatched = (board[i][j]->getGemKind() == board[i + 1][j]->getGemKind() && board[i][j]->getGemKind() == board[i + 2][j]->getGemKind());
				bool gemsUnfrozen = (!iceBlockBoard[i][j].getIsFrozen() && !iceBlockBoard[i + 1][j].getIsFrozen() && !iceBlockBoard[i + 2][j].getIsFrozen());
				bool matchFounded = false;

				if (gemsMatched && gemsUnfrozen)
				{
					board[i][j]->mark();
					board[i + 1][j]->mark();
					board[i + 2][j]->mark();
					matching = true;
					cout << "Match encontrado en (" << i << ", " << j << "), (" << i + 1 << ", " << j << "), (" << i + 2 << ", " << j << ")" << endl;
				}
			}
		}
	}
	return matching;
}

bool Board::hitIceAndGems()
{
	bool hitting = false;

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j]->isMarked() && !iceBlockBoard[i][j].getIsFrozen())
			{
				if (i > 0 && iceBlockBoard[i - 1][j].getIsFrozen())
				{
					iceBlockBoard[i - 1][j].markIce();
				}
				if (i < boardRows - 1 && iceBlockBoard[i + 1][j].getIsFrozen())
				{
					iceBlockBoard[i + 1][j].markIce();
				}
				if (j > 0 && iceBlockBoard[i][j - 1].getIsFrozen())
				{
					iceBlockBoard[i][j - 1].markIce();
				}
				if (j < boardCols - 1 && iceBlockBoard[i][j + 1].getIsFrozen())
				{
					iceBlockBoard[i][j + 1].markIce();
				}
			}
		}
	}

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (iceBlockBoard[i][j].isMarkedIce())
			{
				bool iceWasBroken = iceBlockBoard[i][j].hit();

				if (iceWasBroken)
				{
					cout << "Se ha roto un bloque de hielo en (" << i << ", " << j << ")." << endl;
					iceBlocksBroken++;
					if (iceBlocksBroken > 2)
					{
						iceBlocksBroken = 2;
					}
					if (board[i][j] != nullptr)
					{
						board[i][j]->mark();
					}
						
				}			
				else
				{
					cout << "Se ha golpeado un bloque de hielo en (" << i << ", " << j << ")." << endl;
				}
				hitting = true;
				iceBlockBoard[i][j].unmarkIce();
			}
		}
	}
	return hitting;
}

void Board::deleteFadedGems()
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr && board[i][j]->isMarked() && board[i][j]->getSprite()->getColor().a == 0)
			{
				board[i][j]->getSprite()->setColor(Color::Transparent);
				board[i][j]->unmark();
			}
		}
	}
}

int Board::processMatches()
{
	int matchesFound = 0;
	totalMatches = 0;

	bool bombExploting = true;
	while (bombExploting)
	{
		bombExploting = false;
		
	}
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr && board[i][j]->isMarked() && board[i][j]->getType() == Gem::GemType::Bomb && board[i][j]->getSprite()->getColor().a == 255)
			{
				board[i][j]->onMatch(this, i, j);
				matchesFound++;
				bombExploting = true;
			}
		}
	}
	
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr && board[i][j]->isMarked() && board[i][j]->getType() == Gem::GemType::Normal && board[i][j]->getSprite()->getColor().a == 255)
			{
				board[i][j]->onMatch(this, i, j);
				matchesFound++;

				if (board[i][j]->getGemKind() == 0)
				{
					diamondsCleared++;
				}
			}
		}
	}

	totalMatches = matchesFound;
	return matchesFound;
}

int Board::explotingGems(int row, int col, int dRow, int dCol, int kind)
{
	int count = 0;
	for (int i = 1; i < boardRows; i++)
	{
		int newRow = row + dRow * i;
		int newCol = col + dCol * i;

		if (newRow < 0 || newRow >= boardRows || newCol < 0 || newCol >= boardCols)
		{
			break;
		}
		if (board[newRow][newCol] == nullptr || !board[newRow][newCol]->isMarked() || board[newRow][newCol]->getGemKind() != kind)
		{
			break;
		}
		count++;
	}
	return count;
}

void Board::bombCreation(int row, int col)
{
	if (!board[row][col]->isMarked() || board[row][col]->getType() == Gem::GemType::Bomb)
	{
		return;
	}
	int kind = board[row][col]->getGemKind();
	int horizontal = 1 + explotingGems(row, col, 0, 1, kind) + explotingGems(row, col, 0, -1, kind);
	int vertical = 1 + explotingGems(row, col, 1, 0, kind) + explotingGems(row, col, -1, 0, kind);

	if (horizontal >= 4 || vertical >= 4)
	{
		board[row][col]->unmark();
		float x = board[row][col]->getSprite()->getPosition().x;
		float y = board[row][col]->getSprite()->getPosition().y;
		delete board[row][col];
		board[row][col] = new bombGem();
		board[row][col]->setGem(bombTexture, 5);
		board[row][col]->setLocation(x, y);	
		board[row][col]->getSprite()->setColor(Color::White);
		cout << "Bomba creada en (" << row << ", " << col << ")." << endl;
	}
}

void Board::drawBoard(RenderWindow& window)
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr)
			{
				board[i][j]->draw(window);
			}
			if (iceBlockBoard[i][j].getIsFrozen() && iceBlockBoard[i][j].getSprite() != nullptr)
			{
				window.draw(*iceBlockBoard[i][j].getSprite());
			}
		}
	}
}

bool Board::gravity()
{
	bool gemsGravity = false;
	for (int j = 0; j < boardCols; j++)
	{
		int emptyRow = -1;

		for (int i = boardRows - 1; i >= 0; i--)
		{
			if (board[i][j] != nullptr && board[i][j]->getSprite()->getColor() == Color::Transparent)
			{
				emptyRow = i;
				break;
			}
		}
		if (emptyRow != -1)
		{
			for (int i = emptyRow - 1; i >= 0; i--)
			{
				if (board[i][j] != nullptr && board[i][j]->getSprite()->getColor() != Color::Transparent)
				{
					swap(board[emptyRow][j], board[i][j]);
					swap(iceBlockBoard[emptyRow][j], iceBlockBoard[i][j]);
					board[emptyRow][j]->move(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + emptyRow * CELL_SIDE_SIZE));
					iceBlockBoard[emptyRow][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + emptyRow * CELL_SIDE_SIZE));
					iceBlockBoard[i][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
					gemsGravity = true;
					break;
				}
			}
		}
	}
	return gemsGravity;
}

bool Board::fillOnTop()
{
	bool newGems = false;

	for (int j = 0; j < boardCols; j++)
	{
		if (board[0][j] != nullptr && board[0][j]->getSprite()->getColor() == Color::Transparent)
		{
			delete board[0][j];
			board[0][j] = new normalGem();
			int random = rand() % GEM_TYPE_QUANTITY;
			board[0][j]->setGem(gemTextures[random], random);
			board[0][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START - CELL_SIDE_SIZE));
			board[0][j]->move(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + 0 * CELL_SIDE_SIZE));
			newGems = true;
		}
	}
	return newGems;
}

void Board::initializeBoard(int row, int col)
{
	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			delete board[i][j];
		}
	}
	board.clear();
	iceBlockBoard.clear();

	boardRows = row;
	boardCols = col;

	board.resize(boardRows, vector<Gem*>(boardCols, nullptr));
	iceBlockBoard.resize(boardRows, vector<iceBlock>(boardCols));

	int random = 0;

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr)
			{
				delete board[i][j];
			}
			board[i][j] = new normalGem();
			random = rand() % GEM_TYPE_QUANTITY;
			board[i][j]->setGem(gemTextures[random], random);
			board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));

			if (iceBlockBoard[i][j].getIsFrozen())
			{
				iceBlockBoard[i][j].setIsFrozen(false);
			}
		}
	}
	int iceLimit = rand() % 2 + 2;

	for (int i = 0; i < iceLimit; i++)
	{
		int iceRow = rand() % boardRows;
		int iceCol = rand() % boardCols;

		if (!iceBlockBoard[iceRow][iceCol].getIsFrozen())
		{
			iceBlockBoard[iceRow][iceCol].setBlock(iceBlockTexture, 2);
			iceBlockBoard[iceRow][iceCol].setLocation(static_cast<float>(BOARD_X_START + iceCol * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + iceRow * CELL_SIDE_SIZE));
		}
	}
}

void Board::clearInitialMatches()
{
	while (match())
	{
		for (int i = 0; i < boardRows; i++) {
			for (int j = 0; j < boardCols; j++) {
				if (board[i][j] != nullptr && board[i][j]->isMarked()) {
					board[i][j]->getSprite()->setColor(Color::Transparent);
					board[i][j]->unmark();
				}
			}
		}
		while (updateBoard());
	}

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr)
			{
				delete board[i][j];
				board[i][j] = new normalGem();
				int random = rand() % GEM_TYPE_QUANTITY;
				board[i][j]->setGem(gemTextures[random], random);
				board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
			}
		}
	}

	while (match())
	{
		for (int i = 0; i < boardRows; i++)
		{
			for (int j = 0; j < boardCols; j++)
			{
				if (board[i][j] != nullptr && board[i][j]->isMarked())
				{
					board[i][j]->getSprite()->setColor(Color::Transparent);
					board[i][j]->unmark();
				}
			}
		}
		while (updateBoard());
	}
}

bool Board::updateBoard()
{
	bool gravityGems = false;

	for (int i = boardRows - 1; i >= 1; i--)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j]->getSprite()->getColor() == Color::Transparent)
			{
				swap(board[i][j], board[i - 1][j]);
				board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
				board[i - 1][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + (i - 1) * CELL_SIDE_SIZE));
				swap(iceBlockBoard[i][j], iceBlockBoard[i - 1][j]);
				iceBlockBoard[i][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
				iceBlockBoard[i - 1][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + (i - 1) * CELL_SIDE_SIZE));
				gravityGems = true;
			}
		}
	}

	for (int j = 0; j < boardCols; j++)
	{
		if (board[0][j]->getSprite()->getColor() == Color::Transparent)
		{
			delete board[0][j];
			board[0][j] = new normalGem();
			int random = rand() % GEM_TYPE_QUANTITY;
			board[0][j]->setGem(gemTextures[random], random);
			board[0][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + 0 * CELL_SIDE_SIZE));
			gravityGems = true;
		}
	}	
	return gravityGems;
}

bool Board::fallingGems()
{
	bool gemsFalling = gravity();
	bool newGems = fillOnTop();
	return gemsFalling || newGems;
}

bool Board::updateAnimations(float dt)
{
	bool isAnimating = false;

	for (int i = 0; i < boardRows; i++)
	{
		for (int j = 0; j < boardCols; j++)
		{
			if (board[i][j] != nullptr)
			{
				if (board[i][j]->update(dt))
				{
					isAnimating = true;
				}
			}
		}
	}
	return isAnimating;
}
#include "Board.h"

Board::Board()
{
	gemTextures[0].loadFromFile("assets\\01.png");
	gemTextures[1].loadFromFile("assets\\02.png");
	gemTextures[2].loadFromFile("assets\\03.png");
	gemTextures[3].loadFromFile("assets\\04.png");
	gemTextures[4].loadFromFile("assets\\05.png");

	iceBlockTexture.loadFromFile("assets\\ice.png");

	srand(time(0));
	int random = 0;
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			board[i][j] = new normalGem();
			random = rand() % GEM_TYPE_QUANTITY;
			board[i][j]->setGem(gemTextures[random], random);
			board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));

		}
	}

	int iceLimit = rand() % 2 + 2;

	for (int i = 0; i < iceLimit; i++)
	{
		int iceRow = rand() % BOARD_ROWS;
		int iceCol = rand() % BOARD_COLS;

		if (!iceBlockBoard[iceRow][iceCol].getIsFrozen())
		{
			iceBlockBoard[iceRow][iceCol].setBlock(iceBlockTexture, 2);
			iceBlockBoard[iceRow][iceCol].setLocation(static_cast<float>(BOARD_X_START + iceCol * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + iceRow * CELL_SIDE_SIZE));
		}
	}

	selectedGemRow = -1;
	selectedGemCol = -1;
	totalMatches = 0;
	diamondsCleared = 0;
	iceBlocksBroken = 0;
	hitCounter = 0;
}

Board::~Board()
{
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			delete board[i][j];
		}
	}
}

Sprite* Board::getGem(int row, int col)
{
	return board[row][col]->getSprite();
}

Sprite* Board::getIceBlock(int row, int col)
{
	return iceBlockBoard[row][col].getSprite();
}

int Board::getGemKind(int row, int col)
{
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
		board[row1][col1]->getSprite()->setColor(Color::White);
		board[row2][col2]->getSprite()->setColor(Color::White);
		board[row1][col1]->setLocation(static_cast<float>(BOARD_X_START + col1 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row1 * CELL_SIDE_SIZE));
		board[row2][col2]->setLocation(static_cast<float>(BOARD_X_START + col2 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row2 * CELL_SIDE_SIZE));
		swapping = true;
	}
	if (!swapping)
	{
		board[row1][col1]->getSprite()->setColor(Color::White);
		cout << "Intercambio no realizado." << endl;
		return swapping;
	}
	else if (!match())
	{
		swap(board[row1][col1], board[row2][col2]);
		board[row1][col1]->setLocation(static_cast<float>(BOARD_X_START + col1 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row1 * CELL_SIDE_SIZE));
		board[row2][col2]->setLocation(static_cast<float>(BOARD_X_START + col2 * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + row2 * CELL_SIDE_SIZE));
		board[row1][col1]->getSprite()->setColor(Color::White);
		board[row2][col2]->getSprite()->setColor(Color::White);
		cout << "Match no encontrado." << endl;
		swapping = false;
	}
	return swapping;
}

bool Board::match()
{
	bool matching = false;

	totalMatches = 0;
	hitCounter = 0;

	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (j < BOARD_COLS - 2)
			{
				if ((board[i][j]->getGemKind() == board[i][j + 1]->getGemKind() && board[i][j]->getGemKind() == board[i][j + 2]->getGemKind()) && !iceBlockBoard[i][j].getIsFrozen())
				{
					cout << "Match encontrado en (" << i << ", " << j << "), (" << i << ", " << j + 1 << "), (" << i << ", " << j + 2 << ")" << endl;
					board[i][j]->mark();
					board[i][j + 1]->mark();
					board[i][j + 2]->mark();
					matching = true;
					if (board[i][j]->getGemKind() == 0)
					{
						diamondsCleared++;
					}
				}
			}

			if (i < BOARD_ROWS - 2)
			{
				if ((board[i][j]->getGemKind() == board[i + 1][j]->getGemKind() && board[i][j]->getGemKind() == board[i + 2][j]->getGemKind()) && !iceBlockBoard[i][j].getIsFrozen())
				{
					cout << "Match encontrado en (" << j << ", " << i << "), (" << j + 1 << ", " << i << "), (" << j + 2 << ", " << i << ")" << endl;
					board[i][j]->mark();
					board[i + 1][j]->mark();
					board[i + 2][j]->mark();
					matching = true;
					if (board[i][j]->getGemKind() == 0)
					{
						diamondsCleared++;
					}
				}
			}
			if (board[i][j]->isMarked())
			{
				totalMatches++;
			}
		}
	}

	if (!matching)
	{
		return matching;
	}
	removeGems();

	return matching;
}

bool Board::hitIceAndGems()
{
	bool hitting = false;

	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (board[i][j]->isMarked() && !iceBlockBoard[i][j].getIsFrozen())
			{
				if (i > 0 && iceBlockBoard[i - 1][j].getIsFrozen())
				{
					iceBlockBoard[i - 1][j].markIce();
				}
				if (i < BOARD_ROWS - 1 && iceBlockBoard[i + 1][j].getIsFrozen())
				{
					iceBlockBoard[i + 1][j].markIce();
				}
				if (j > 0 && iceBlockBoard[i][j - 1].getIsFrozen())
				{
					iceBlockBoard[i][j - 1].markIce();
				}
				if (j < BOARD_COLS - 1 && iceBlockBoard[i][j + 1].getIsFrozen())
				{
					iceBlockBoard[i][j + 1].markIce();
				}
			}
		}
	}

	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (iceBlockBoard[i][j].isMarkedIce())
			{
				iceBlockBoard[i][j].hit();

				if (!iceBlockBoard[i][j].getIsFrozen())
				{
					cout << "Se ha roto un bloque de hielo en (" << i << ", " << j << ")." << endl;
					iceBlocksBroken++;
					if (iceBlocksBroken > 2)
					{
						iceBlocksBroken = 2;
					}
				}
				else
				{
					cout << "Se ha golpeado un bloque de hielo en (" << i << ", " << j << ")." << endl;
				}
				hitting = true;
			}

		}
	}


	return hitting;
}

bool Board::removeGems()
{
	bool removing = false;

	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (board[i][j]->isMarked())
			{
				board[i][j]->getSprite()->setColor(Color::Transparent);
			}
		}
	}
	return removing;
}

void Board::initializeBoard()
{
	match();
	while (match())
	{
		for (int i = 0; i < BOARD_ROWS; i++)
		{
			for (int j = 0; j < BOARD_COLS; j++)
			{
				if (board[i][j]->getSprite()->getColor() == Color::Transparent)
				{
					delete board[i][j];
					board[i][j] = new normalGem();
					int random = rand() % GEM_TYPE_QUANTITY;
					board[i][j]->setGem(gemTextures[random], random);
					board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));

				}
			}
		}
	}
}

bool Board::updateBoard()
{
	bool gravity = false;
	match();

	for (int i = BOARD_ROWS - 1; i >= 1; i--)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (board[i][j]->getSprite()->getColor() == Color::Transparent)
			{
				swap(board[i][j], board[i - 1][j]);
				board[i][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
				board[i - 1][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + (i - 1) * CELL_SIDE_SIZE));
				swap(iceBlockBoard[i][j], iceBlockBoard[i - 1][j]);
				iceBlockBoard[i][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + i * CELL_SIDE_SIZE));
				iceBlockBoard[i - 1][j].setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + (i - 1) * CELL_SIDE_SIZE));
				gravity = true;
			}
		}
	}

	for (int j = 0; j < BOARD_COLS; j++)
	{
		if (board[0][j]->getSprite()->getColor() == Color::Transparent)
		{
			delete board[0][j];
			board[0][j] = new normalGem();
			int random = rand() % GEM_TYPE_QUANTITY;
			board[0][j]->setGem(gemTextures[random], random);
			board[0][j]->setLocation(static_cast<float>(BOARD_X_START + j * CELL_SIDE_SIZE), static_cast<float>(BOARD_Y_START + 0 * CELL_SIDE_SIZE));
			gravity = true;
		}
	}
	return gravity;
}

bool Board::isAnimating()
{
	bool anim = false;
	return anim;
}
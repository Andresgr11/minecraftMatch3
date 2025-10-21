#include "Board.h"
#include "Game.h"

Board::Board()
{
	gemTextures[0].loadFromFile("assets\\01.png");
	gemTextures[1].loadFromFile("assets\\02.png");
	gemTextures[2].loadFromFile("assets\\03.png");
	gemTextures[3].loadFromFile("assets\\04.png");
	gemTextures[4].loadFromFile("assets\\05.png");

	srand(time(0));
	int random = 0;
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			random = rand() % GEM_TYPE_QUANTITY;
			board[i][j].setGem(gemTextures[random], random);
			board[i][j].setLocation(BOARD_X_START + j * CELL_SIDE_SIZE, BOARD_Y_START + i * CELL_SIDE_SIZE);
		}
	}
}

Sprite* Board::getGem(int row, int col)
{
	return board[row][col].getSprite();
}

void Board::setLocation(int row, int col, float x, float y)
{
	board[row][col].setLocation(x, y);
}

void Board::selectGem(int row, int col)
{
	cout << "Gema seleccionada en (" << row << ", " << col << ") del tipo #" << board[row][col].getGemKind() << endl;
	board[row][col].getSprite()->setColor(Color::Blue);
	selectedGemCol = col;
	selectedGemRow = row;
}

bool Board::swapping(int row1, int col1, int row2, int col2)
{
	bool swapping = false;

	if ((abs(row1 - row2) == 1 && col1 == col2) || (abs(col1 - col2) == 1 && row1 == row2))
	{				
		swap(board[row1][col1], board[row2][col2]);
		cout << "Gemas intercambiadas en: (" << row1 << ", " << col1 << ") y (" << row2 << ", " << col2 << ")." << endl;
		board[row1][col1].getSprite()->setColor(Color::White);
		board[row2][col2].getSprite()->setColor(Color::White);
		board[row1][col1].setLocation(BOARD_X_START + col1 * CELL_SIDE_SIZE, BOARD_Y_START + row1 * CELL_SIDE_SIZE);
		board[row2][col2].setLocation(BOARD_X_START + col2 * CELL_SIDE_SIZE, BOARD_Y_START + row2 * CELL_SIDE_SIZE);
		swapping = true;
		
	}
	if (!swapping)
	{
		board[row1][col1].getSprite()->setColor(Color::White);
		cout << "Intercambio no realizado." << endl;
		
	}
	else if (!match())
	{
		swap(board[row1][col1], board[row2][col2]);
		board[row1][col1].setLocation(BOARD_X_START + col1 * CELL_SIDE_SIZE, BOARD_Y_START + row1 * CELL_SIDE_SIZE);
		board[row2][col2].setLocation(BOARD_X_START + col2 * CELL_SIDE_SIZE, BOARD_Y_START + row2 * CELL_SIDE_SIZE);
		board[row1][col1].getSprite()->setColor(Color::White);
		board[row2][col2].getSprite()->setColor(Color::White);
		cout << "Match no encontrado." << endl;
		swapping = false;
	}
	return swapping;
}

bool Board::match()
{
	bool matching = false;
	totalMatches = 0;
	
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS - 2; j++)
		{
			if (board[i][j].getGemKind() == board[i][j + 1].getGemKind() && board[i][j].getGemKind() == board[i][j + 2].getGemKind())
			{
				cout << "Match encontrado en (" << i << ", " << j << "), (" << i << ", " << j + 1 << "), (" << i << ", " << j + 2 << ")" << endl;
				board[i][j].getSprite()->setColor(Color::Black);
				board[i][j + 1].getSprite()->setColor(Color::Black);
				board[i][j + 2].getSprite()->setColor(Color::Black);
				matching = true;
			}
			if (board[j][i].getGemKind() == board[j + 1][i].getGemKind() && board[j][i].getGemKind() == board[j + 2][i].getGemKind())
			{
				cout << "Match encontrado en (" << j << ", " << i << "), (" << j + 1 << ", " << i << "), (" << j + 2 << ", " << i << ")" << endl;
				board[j][i].getSprite()->setColor(Color::Black);
				board[j + 1][i].getSprite()->setColor(Color::Black);
				board[j + 2][i].getSprite()->setColor(Color::Black);
				matching = true;
			}
			if (board[j][i].getSprite()->getColor() == Color::Black)
			{
				totalMatches++;
			}
		}
	}
	return matching;
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
				if (board[i][j].getSprite()->getColor() == Color::Black)
				{
					board[i][j] = Gem();
					int random = rand() % GEM_TYPE_QUANTITY;
					board[i][j].setGem(gemTextures[random], random);
					board[i][j].setLocation(BOARD_X_START + j * CELL_SIDE_SIZE, BOARD_Y_START + i * CELL_SIDE_SIZE);
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
			if (board[i][j].getSprite()->getColor() == Color::Black)
			{
				swap(board[i][j], board[i - 1][j]);
				board[i][j].setLocation(BOARD_X_START + j * CELL_SIDE_SIZE, BOARD_Y_START + i * CELL_SIDE_SIZE);
				board[i - 1][j].setLocation(BOARD_X_START + j * CELL_SIDE_SIZE, BOARD_Y_START + (i - 1) * CELL_SIDE_SIZE);
				gravity = true;
			}
		}
	}

	for (int j = 0; j < BOARD_COLS; j++)
	{
		if (board[0][j].getSprite()->getColor() == Color::Black)
		{
			board[0][j] = Gem();
			int random = rand() % GEM_TYPE_QUANTITY;
			board[0][j].setGem(gemTextures[random], random);
			board[0][j].setLocation(BOARD_X_START + j * CELL_SIDE_SIZE, BOARD_Y_START + 0 * CELL_SIDE_SIZE);
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
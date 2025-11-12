#include "bombGem.h"
#include "Board.h"

void bombGem::draw(RenderWindow& window)
{
	Gem::draw(window);
}

void bombGem::onMatch(Board* board, int row, int col)
{
	cout << "TNT explotando en (" << row << ", " << col << ")." << endl;
	Gem::onMatch(board, row, col);

	int rows = board->getRows();
	int cols = board->getCols();

	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			if (i >= 0 && i < rows && j >= 0 && j < cols)
			{
				Gem* adyacent = board->getGemType(i, j);
				if (adyacent != nullptr)
				{
					adyacent->mark();
				}
			}
		}
	}
}

Gem::GemType bombGem::getType() const
{
	return GemType::Bomb;
}
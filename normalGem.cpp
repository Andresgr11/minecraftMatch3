#include "normalGem.h"
#include "Board.h"

void normalGem::draw(RenderWindow& window)
{
	Gem::draw(window);
}

void normalGem::onMatch(Board* board, int row, int col)
{
	Gem::onMatch(board, row, col);
}

Gem::GemType normalGem::getType() const
{
	return GemType::Normal;
}

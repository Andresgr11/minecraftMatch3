#pragma once
#include "Gem.h"
#include "Board.h"

class normalGem : public Gem
{
public:
	virtual void draw(RenderWindow& window) override;
	virtual void onMatch(Board* board, int row, int col) override;
	virtual GemType getType() const override;
};
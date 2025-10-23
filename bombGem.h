#pragma once
#include "Gem.h"
class bombGem : public Gem
{
public:
	virtual GemType getType() const override;
};
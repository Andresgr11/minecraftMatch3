#pragma once
#include "Gem.h"
class normalGem : public Gem
{
public:
	virtual void draw() override;
	virtual void onMatch() override;
	virtual GemType getType() const override;
};
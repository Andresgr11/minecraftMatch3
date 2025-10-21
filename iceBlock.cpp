#include "iceBlock.h"

iceBlock::iceBlock()
{
	iceTexture.loadFromFile("assets\\ice.png");
	iceSprite = new Sprite(iceTexture);
	isFrozen = true;
	iceCounter = 2;
}

void iceBlock::setLocation(float x, float y)
{
	iceSprite->setPosition(Vector2f(x, y));
}

Sprite* iceBlock::getSprite()
{
	return iceSprite;
}

bool iceBlock::getIsFrozen() const
{
	return isFrozen;
}

void iceBlock::setIsFrozen(bool frozen)
{
	isFrozen = frozen;
}
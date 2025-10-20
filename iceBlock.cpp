#include "iceBlock.h"

iceBlock::iceBlock()
{
	iceSprite = nullptr;
	isFrozen = true;
	iceCounter = 2;
}

void iceBlock::setIceBlock(Texture& itexture)
{
	iceSprite = new Sprite(itexture);
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
#include "iceBlock.h"

iceBlock::iceBlock()
{
	iceSprite = nullptr;
	isFrozen = false;
	iceCounter = 0;
	markedForHit = false;
}

void iceBlock::setLocation(float x, float y)
{
	if (isFrozen && iceSprite != nullptr)
	{
		iceSprite->setPosition(Vector2f(x, y));
	}
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
	if (!isFrozen && iceSprite != nullptr)
	{
		delete iceSprite;
		iceSprite = nullptr;
		iceCounter = 0;
	}
}

void iceBlock::markIce()
{
	markedForHit = true;
}

void iceBlock::unmarkIce()
{
	markedForHit = false;
}

bool iceBlock::isMarkedIce() const
{
	return markedForHit;
}

void iceBlock::setBlock(Texture& gtexture, int health)
{
	if (!isFrozen)
	{
		iceSprite = new Sprite(gtexture);
		isFrozen = true;
		iceCounter = health;
	}
}

bool iceBlock::hit()
{
	bool hitIce = false;
	
	if (!isFrozen)
	{
		return hitIce;
	}

	iceCounter--;

	if (iceCounter <= 0)
	{
		isFrozen = false;
		if (iceSprite != nullptr)
		{
			delete iceSprite;
			iceSprite = nullptr;
		}
		hitIce = true;
	}
	return hitIce;
}
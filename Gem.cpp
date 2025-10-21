#include "Gem.h"

Gem::Gem()
{
	gemSprite = nullptr;
	kind = -1;
}

void Gem::setGem(Texture& gtexture, int gKind)
{
	gemSprite = new Sprite(gtexture);
	kind = gKind;
}

void Gem::setLocation(float x, float y)
{
	gemSprite->setPosition(Vector2f(x, y));
}

Sprite* Gem::getSprite()
{
	return gemSprite;
}

int Gem::getGemKind() const
{
	return kind;
}

void Gem::setGemKind(int gKind)
{
	kind = gKind;
}

bool Gem::animating(float x, float y)
{
	bool anim = false;

	return anim;
}

#include "Gem.h"
#include "normalGem.h"
#include "lineGem.h"
#include "bombGem.h"

Gem::Gem()
{
	gemSprite = nullptr;
	kind = 0;
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

void normalGem::functions()
{
}

void lineGem::functions()
{
}

void bombGem::functions()
{
}
#include "Gem.h"

Gem::Gem()
{  
	gemSprite = nullptr;
	kind = -1;
	markedForMatch = false;
}

Gem::~Gem()
{
	deleteGem();
}

void Gem::deleteGem()
{
	if (gemSprite != nullptr)
	{
		delete gemSprite;
		gemSprite = nullptr;
	}
}

void Gem::setGem(Texture& gtexture, int gKind)
{
	gemSprite = new Sprite (gtexture);
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

Gem::GemType Gem::getType() const
{
	return GemType::Normal;
}

int Gem::getGemKind() const
{
	return kind;
}

void Gem::setGemKind(int gKind)
{
	kind = gKind;
}

void Gem::mark()
{
	markedForMatch = true;
}

void Gem::unmark()
{
	markedForMatch = false;
}

bool Gem::isMarked() const
{
	return markedForMatch;
}

bool Gem::animating(float x, float y)
{
	bool anim = false;
	return anim;
}
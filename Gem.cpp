#include "Gem.h"

Gem::Gem()
{  
	gemSprite = nullptr;
	kind = -1;
	markedForMatch = false;
	currentPos = Vector2f(0, 0);
	targetPos = Vector2f(0, 0);
	alpha = 255.0f;
	isFading = false;
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

void Gem::draw()
{

}

void Gem::onMatch()
{


}

void Gem::setGem(Texture& gtexture, int gKind)
{
	gemSprite = new Sprite (gtexture);
	kind = gKind;
	alpha = 255.0f;
	isFading = false;
	gemSprite->setColor(Color(255, 255, 255, 255));
}

void Gem::setLocation(float x, float y)
{
	currentPos = Vector2f(x, y);
	targetPos = Vector2f(x, y);
	if (gemSprite != nullptr)
	{
		gemSprite->setPosition(currentPos);
	}
}

void Gem::move(float x, float y)
{
	targetPos = Vector2f(x, y);	
}

void Gem::fadeOut()
{
	isFading = true;
}

bool Gem::update(float dt)
{
	if (gemSprite == nullptr)
	{
		return false;
	}

	bool animating = false;
	float speed = GEM_MOVE_SPEED;
	float fadeSpeed = GEM_FADE_SPEED;

	Vector2f posDifference = targetPos - currentPos;
	float distance = sqrt(posDifference.x * posDifference.x + posDifference.y * posDifference.y);

	if (distance > 1.0f)
	{
		animating = true;
		Vector2f direction = posDifference / distance;
		currentPos += direction * speed * dt;
		
		if (sqrt(pow(targetPos.x - currentPos.x, 2) + pow(targetPos.y - currentPos.y, 2)) < 1.0f || (direction.x * (targetPos.x - currentPos.x) < 0 || direction.y * (targetPos.y - currentPos.y) < 0))
		{
			currentPos = targetPos;
		}
	
	}
	else
	{
		currentPos = targetPos;
	}
	gemSprite->setPosition(currentPos);

	if (isFading)
	{
		animating = true;
		alpha -= 255.f * fadeSpeed * dt;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			isFading = false;
		}
		gemSprite->setColor(Color(255, 255, 255, (uint8_t)alpha));
	}

	return animating;
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
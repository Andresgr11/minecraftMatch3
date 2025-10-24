#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdint>

using namespace sf;
using namespace std;

class Gem
{
private:
	Sprite* gemSprite;
	int kind;
	bool markedForMatch;
	Vector2f currentPos;
	Vector2f targetPos;
	float alpha;
	bool isFading;
public:
	enum class GemType { Normal, Bomb };
	Gem();
	virtual ~Gem();
	virtual void deleteGem();
	virtual void draw(RenderWindow& window);
	virtual void onMatch(Board* board, int row, int col);
	void setGem(Texture& gtexture, int gKind);
	virtual void setLocation(float x, float y);
	virtual void move(float x, float y);
	virtual void fadeOut();
	virtual bool update(float dt);
	virtual Sprite* getSprite();
	virtual GemType getType() const;
	virtual int getGemKind() const;
	virtual void setGemKind(int gKind);
	virtual void mark();
	virtual void unmark();
	virtual bool isMarked() const;
};
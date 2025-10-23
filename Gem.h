#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>

using namespace sf;
using namespace std;

class Gem
{
private:
	Sprite* gemSprite;
	int kind;
	bool markedForMatch;
public:
	Gem();
	virtual void deleteGem();
	enum class state { idle, moving };
	void setGem(Texture& gtexture, int gKind);
	virtual void setLocation(float x, float y);
	virtual Sprite* getSprite();
	virtual int getGemKind() const;
	virtual void setGemKind(int gKind);
	virtual void mark();
	virtual void unmark();
	virtual bool isMarked() const;
	virtual bool animating(float x, float y);
};
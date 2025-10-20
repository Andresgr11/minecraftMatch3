#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

class Gem
{
private:
	Sprite* gemSprite;
	int kind;
public:
	Gem();
	virtual void setGem(Texture& gtexture, int gKind);
	virtual void setLocation(float x, float y);
	virtual Sprite* getSprite();
	virtual int getGemKind() const;
	virtual void setGemKind(int gKind);
};
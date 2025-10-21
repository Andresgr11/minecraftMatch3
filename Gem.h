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
	void setGem(Texture& gtexture, int gKind);
	void setLocation(float x, float y);
	Sprite* getSprite();
	int getGemKind() const;
	void setGemKind(int gKind);
	bool animating(float x, float y);
};
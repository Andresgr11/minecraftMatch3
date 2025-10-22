#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

class iceBlock
{
private:
	Sprite* iceSprite;
	bool isFrozen;
public:
	iceBlock();
	int iceCounter;
	void setLocation(float x, float y);
	Sprite* getSprite();
	bool getIsFrozen() const;
	void setIsFrozen(bool frozen);
	void setBlock(Texture& gtexture, int health = 2);
	bool hit();
};


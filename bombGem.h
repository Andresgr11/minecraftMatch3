#pragma once
#include "Gem.h"
class bombGem : public Gem
{
public:

	virtual void setGem(Texture& gtexture, int gKind);
	virtual void setLocation(float x, float y);
	virtual Sprite* getSprite();
	virtual int getGemKind() const;
	virtual void setGemKind(int gKind);
	virtual void functions() override;
};


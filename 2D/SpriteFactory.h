#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include "Sprite.h"
#include "memory.h"

class SpriteFactory {


private:

	std::vector<std::unique_ptr<Sprite2D>> sprites;

public:

	Sprite2D* createSprite(const float x1, const float y1, const float x2, const float y2) {

		Sprite2D* sprite = new Sprite2D();
		sprites.push_back(std::make_unique(sprite));

		sprite->setRect(x1,y1, x2,y2);

		return sprite;

	}

};

#endif // SPRITEFACTORY_H

#ifndef SPRITEFACTORY_H
#define SPRITEFACTORY_H

#include "Sprite.h"
#include "memory.h"

class SpriteFactory {


private:

	std::vector<std::unique_ptr<Sprite2D>> sprites;

public:

	Sprite2D* createSprite() {
		Sprite2D* sprite = new Sprite2D();
		sprites.push_back(std::make_unique(sprite));
		return sprite;
	}

	Sprite2D* createSprite(const Rect& rect) {
		Sprite2D* sprite = createSprite();
		sprite->setRect(rect);
		return sprite;
	}

};

#endif // SPRITEFACTORY_H

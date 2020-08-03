#include "DestroyedCreature.h"

SDL_Surface* DestroyedCreature::destroyedCreatureImage = nullptr;

DestroyedCreature::DestroyedCreature() {}

DestroyedCreature::DestroyedCreature(Position* position)
{
	this->position = position;
	this->creationTime = std::chrono::steady_clock::now();
	this->auxiliaryCounter = 0;
}

void DestroyedCreature::loadDestroyedCreatureImage(SDL_Surface* screen)
{
	this->destroyedCreatureImage = loadPNG("./img/destroyed_creature.png", screen);
}

void DestroyedCreature::draw(SDL_Surface* screen, int beginningOfCamera)
{
	drawSurface(screen, this->destroyedCreatureImage, this->position->getX() - beginningOfCamera, this->position->getY());
}

bool DestroyedCreature::shouldBeRemoved()
{
	std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();
	return (this->creationTime + std::chrono::milliseconds(2000) < timePoint);
}

void DestroyedCreature::slide()
{
	++this->auxiliaryCounter;

	if (this->auxiliaryCounter & 1) {
		if (this->auxiliaryCounter <= 12) {
			this->position->setY(this->position->getY() - 3);
		}
		else if (this->auxiliaryCounter >= 13 && this->auxiliaryCounter <= 24) {
			this->position->setX(this->position->getX() + 3);
			this->position->setY(this->position->getY() - 2);
		}
		else if (this->auxiliaryCounter >= 25 && this->auxiliaryCounter <= 32) {
			this->position->setX(this->position->getX() + 3);
		}
		else if (this->auxiliaryCounter >= 33 && this->auxiliaryCounter <= 44) {
			this->position->setX(this->position->getX() + 3);
			this->position->setY(this->position->getY() + 2);
		}
		else {
			this->position->setY(this->position->getY() + 3);
			if (this->auxiliaryCounter % 3 == 0) {
				this->position->setX(this->position->getX() + 1);
			}
		}
	}
}


#include "IndependentLivingObject.h"

void IndependentLivingObject::makeHorizontalMove(World& world)
{
	int alignment = getAlignmentIfCollisionOccursDuringMovement(movement->getDirection(), movement->getSpeed(), this, world);
	int distance = movement->getSpeed() - alignment;
	if (movement->getDirection() == Left) {
		distance *= -1;
	}
	position->setX(position->getX() + distance);

	if (alignment > 0) {
		movement->setDirection(movement->getDirection() == Right ? Left : Right);
	}
}

void IndependentLivingObject::makeDiagonalMove(World& world)
{
	int alignment = getAlignmentIfCollisionOccursDuringVerticalMovement(Down, movement->getVerticalSpeed(), this, world);
	int verticalDistance = movement->getVerticalSpeed() - alignment;
	position->setY(position->getY() + verticalDistance);

	alignment = getAlignmentIfCollisionOccursDuringMovement(movement->getDirection(), movement->getSpeed(), this, world);
	int distance = movement->getSpeed() - alignment;
	if (movement->getDirection() == Left) {
		distance *= -1;
	}
	position->setX(position->getX() + distance);
}

#include "RedTurtle.h"

#include "Movement.h"
#include "Size.h"
#include "Position.h"
#include "CollisionHandling.h"
#include "SDL_Utility.h"
#include "World.h"
#include "LayoutStyle.h"


std::array<SDL_Surface*, 6> RedTurtle::redTurtleImages;

int RedTurtle::computeIndex() const
{
	if (flying) {
		return 4;
	}
	else {
		return (movement.getDirection() == Direction::Left ? 0 : 2);
	}
}

void RedTurtle::fly()
{
	++stepsCounter;
	if (stepsCounter % 3 == 0) {
		int verticalDistance = movement.getVerticalSpeed();
		if (movement.getVerticalDirection() == Direction::Up) {
			verticalDistance *= -1;
		}
		position.setY(position.getY() + verticalDistance);
	}

	if (stepsCounter == 180) {
		movement.setVerticalDirection(
			movement.getVerticalDirection() == Direction::Down ? Direction::Up : Direction::Down);

		stepsCounter = 0;
	}
}

void RedTurtle::patrol(World &world)
{
	++stepsCounter;
	if (stepsCounter % 3 == 0) {
		if (!isCharacterStandingOnTheBlock(*this, world)) {
			makeDiagonalMove(world);
		}
		else {
			int alignment = getAlignmentForHorizontalMove(movement.getDirection(), movement.getSpeed(), *this, world);
			int distance = movement.getSpeed() - alignment;
			if (movement.getDirection() == Direction::Left) {
				distance *= -1;
			}
			position.setX(position.getX() + distance);

			if (alignment > 0) {
				movement.setDirection(
					movement.getDirection() == Direction::Right ? Direction::Left : Direction::Right);
			}
		}
	}

	if (stepsCounter == 300) {
		movement.setDirection(movement.getDirection() == Direction::Right ? Direction::Left : Direction::Right);
		stepsCounter = 0;
	}
}

void RedTurtle::chooseModel()
{
	++changeModelCounter;
	if (changeModelCounter % 90 == 0) {
		model += (model & 1 ? 1 : -1);
	}
}

RedTurtle::RedTurtle(Position position, bool flying)
{
	size = Size(26, 38);
	movement = Movement(1, 3, Direction::Left);
	this->position = position;
	this->flying = flying;
	model = 1;
	stepsCounter = 0;
	changeModelCounter = 0;

	if (flying) {
		movement.setVerticalDirection(Direction::Up);
		movement.setVerticalSpeed(2);
	}
}

bool RedTurtle::isFlying() const
{
	return flying;
}

void RedTurtle::loseFlyingAbility()
{
	flying = false;
	stepsCounter = 0;
	movement.setVerticalDirection(Direction::None);
	movement.setDirection(Direction::Left);
	movement.setVerticalSpeed(2);
	position.setY(position.getY() + 10);
}

void RedTurtle::setMoveDirection(Direction direction)
{
	movement.setDirection(direction);
}

void RedTurtle::loadTurtleImages(SDL_Surface* display)
{
	for (std::size_t i = 0; i < redTurtleImages.size(); ++i) {
		std::string filename = "./img/red_turtle";
		filename += std::to_string(i + 1);
		filename += ".png";
		redTurtleImages[i] = loadPNG(filename, display);
	}
}

void RedTurtle::draw(SDL_Surface* display, int beginningOfCamera, int endOfCamera) const
{
	if (position.getX() > beginningOfCamera - 100 && position.getX() < endOfCamera + 100) {
		SDL_Surface* redTurtleImg = nullptr;
		redTurtleImg = redTurtleImages[computeIndex() + (model - 1)];
		drawSurface(display, redTurtleImg, position.getX() - beginningOfCamera, position.getY());
	}
}

void RedTurtle::move(World &world)
{
	if (flying) {
		fly();
	}
	else {
		if (movement.getDirection() != Direction::None) {
			patrol(world);
		}
	}
	chooseModel();
}

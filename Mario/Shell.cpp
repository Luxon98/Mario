#include "Shell.h"

#include "Size.h"
#include "Movement.h"
#include "Position.h"
#include "CollisionHandling.h"
#include "SDL_Utility.h"
#include "World.h"
#include "LayoutStyle.h"
#include "UtilityFunctions.h"
#include "Player.h"


std::array<SDL_Surface*, 3> Shell::shellImages;

int Shell::computeImageIndex() const
{
	if (!red) {
		return (World::LAYOUT_STYLE == LayoutStyle::OpenWorld ? 0 : 1);
	}
	else {
		return 2;
	}
}

Shell::Shell(Position position, bool red)
{
	size = Size(32, 28);
	movement = Movement(3, 2, Direction::None);
	this->position = position;
	this->red = red;
	healthPoints = 1;
	stepsCounter = 0;
	stateTime = std::chrono::steady_clock::now();
	active = false;
}

void Shell::loadShellImage(SDL_Surface* display)
{
	for (std::size_t i = 0; i < shellImages.size(); ++i) {
		std::string filename = "./img/other_imgs/shell";
		filename += std::to_string(i + 1);
		filename += ".png";
		shellImages[i] = loadPNG(filename, display);
	}
}

bool Shell::isResistantToImmortalPlayer() const
{
	return !active;
}

bool Shell::isActiveShell() const
{
	return active;
}

bool Shell::isRed() const
{
	return red;
}

bool Shell::shouldTurnIntoTurtle() const
{
	if (red || active) {
		return false;
	}

	auto timePoint = std::chrono::steady_clock::now();
	return (stateTime + std::chrono::milliseconds(20000) < timePoint);
}

void Shell::changeActiveState(Direction direction)
{
	movement.setDirection(direction);
	active = (movement.getDirection() != Direction::None);

	int shift = determineShift(*this, 3);
	position.setX(position.getX() + shift);

	stateTime = std::chrono::steady_clock::now();
}

void Shell::draw(SDL_Surface* display, int beginningOfCamera, int endOfCamera) const
{
	if (isWithinRangeOfCamera(beginningOfCamera, endOfCamera)) {
		SDL_Surface* shellImg = shellImages[computeImageIndex()];
		drawSurface(display, shellImg, position.getX() - beginningOfCamera, position.getY());
	}
}

void Shell::move(World &world)
{
	if (movement.getDirection() != Direction::None && stepsCounter & 1) {
		if (isCharacterStandingOnSomething(*this, world)) {
			moveHorizontally(world);
		}
		else {
			moveDiagonally(world);
		}
	}
	else if (!isCharacterStandingOnSomething(*this, world) && stepsCounter & 1) {
		moveDiagonally(world);
	}

	++stepsCounter;
}

void Shell::crush(World &world, int index)
{
	if (active) {
		changeActiveState(Direction::None);
	}
	else {
		const Player& player = world.getPlayer();
		Direction direction = (player.getX() <= getX() ? Direction::Right : Direction::Left); 	// to simplify
		changeActiveState(direction);
	}
}

void Shell::destroy(World &world, Direction direction)
{
	world.addDestroyedTurtle(position, direction, red);
}

void Shell::performSpecificActions(World &world, int index)
{
	if (shouldTurnIntoTurtle()) {
		world.addTurtle(position);
		world.deleteNpc(index);
	}
	else if (world.isObjectOutsideCamera(*this)) {
		world.deleteNpc(index);
	}
}
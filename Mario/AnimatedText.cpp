#include "AnimatedText.h"

#include "Position.h"
#include "SDL_Utility.h"


std::array<SDL_Surface*, 6> AnimatedText::animatedTextImages;

int AnimatedText::computeImageIndex() const
{
	return (static_cast<int>(type) - 1);
}

AnimatedText::AnimatedText(TextType type, Position position)
{
	this->type = type;
	this->position = position;
	auxiliaryCounter = 0;
}

void AnimatedText::loadAnimatedTextImages(SDL_Surface* display)
{
	animatedTextImages[0] = loadPNG("./img/te_imgs/100.png", display);
	animatedTextImages[1] = loadPNG("./img/te_imgs/200.png", display);
	animatedTextImages[2] = loadPNG("./img/te_imgs/400.png", display);
	animatedTextImages[3] = loadPNG("./img/te_imgs/1000.png", display);
	animatedTextImages[4] = loadPNG("./img/te_imgs/5000.png", display);
	animatedTextImages[5] = loadPNG("./img/te_imgs/1UP.png", display);
}

void AnimatedText::draw(SDL_Surface* display, int beginningOfCamera, int endOfCamera) const
{
	if (position.getX() > beginningOfCamera - 60 && position.getX() < endOfCamera + 60) {
		SDL_Surface* animatedTextImg = animatedTextImages[computeImageIndex()];
		drawSurface(display, animatedTextImg, position.getX() - beginningOfCamera, position.getY());
	}
}

bool AnimatedText::shouldBeRemoved() const
{
	auto timePoint = std::chrono::steady_clock::now();
	return (auxiliaryCounter > 150);
}

void AnimatedText::slide()
{
	++auxiliaryCounter;
	if (auxiliaryCounter % 3 == 0) {
		position.setY(position.getY() - 1);
	}
}


#include "GameFunctions.h"

bool isDifferenceInInterval(int difference, int begin, int shift, int repetitions)
{
	for (int i = begin, j = 0; j < repetitions; i += shift, j++) {
		if (difference >= i && difference <= i + 150) {
			return true;
		}
	}

	return false;
}

void loadBonusObjectsImages(SDL_Surface* display)
{
	Flower tempFlower = Flower();
	tempFlower.loadFlowerImages(display);

	Mushroom tempMushroom = Mushroom();
	tempMushroom.loadMushroomImages(display);

	Star tempStar = Star();
	tempStar.loadStarImages(display);

	Coin tempCoin = Coin();
	tempCoin.loadCoinImages(display);
}

void loadInanimateObjectImages(SDL_Surface* display)
{
	Bush tempBush = Bush();
	tempBush.loadBushImages(display);

	Cloud tempCloud = Cloud();
	tempCloud.loadCloudImages(display);

	Castle tempCastle = Castle();
	tempCastle.loadCastleImages(display);

	Flag tempFlag = Flag();
	tempFlag.loadFlagImages(display);
}

void loadLivingObjectImages(SDL_Surface* display)
{
	Creature tempCreature = Creature();
	tempCreature.loadCreatureImages(display);

	Turtle tempTurtle = Turtle();
	tempTurtle.loadTurtleImages(display);

	Shell tempShell = Shell();
	tempShell.loadShellImage(display);

	FireBall tempFireBall = FireBall();
	tempFireBall.loadFireBallImages(display);

	Player tempPlayer = Player();
	tempPlayer.loadPlayerImages(display);
}

void loadTemporaryObjectImages(SDL_Surface* display)
{
	CrushedCreature tempCrushedCreature = CrushedCreature();
	tempCrushedCreature.loadCrushedCreatureImage(display);

	DestroyedCreature tempDestroyedCreature = DestroyedCreature();
	tempDestroyedCreature.loadDestroyedCreatureImage(display);

	DestroyedTurtle tempDestroyedTurtle = DestroyedTurtle();
	tempDestroyedTurtle.loadDestroyedTurtleImage(display);

	Explosion tempExplosion = Explosion();
	tempExplosion.loadExplosionImage(display);

	Shards tempShards = Shards();
	tempShards.loadShardsImages(display);

	AnimatedCoin tempAnimatedCoin = AnimatedCoin();
	tempAnimatedCoin.loadAnimatedCoinImages(display);

	AnimatedText tempAnimatedText = AnimatedText();
	tempAnimatedText.loadAnimatedTextImages(display);
}

void loadBlockImages(SDL_Surface* display)
{
	Block tempBlock = Block();
	tempBlock.loadBlockImages(display);
}

void loadImages(SDL_Surface* display)
{
	loadBonusObjectsImages(display);
	loadInanimateObjectImages(display);
	loadLivingObjectImages(display);
	loadTemporaryObjectImages(display);
	loadBlockImages(display);
}

void resetGame(KeyboardController controller, Player* player, World& world, Screen* screen, bool * playerState)
{
	controller.clearKeysState();
	player->reborn();
	*playerState = true;
	Level::setFirstLevel(world);
	screen->resetScreen();
}

void runGame()
{
	bool playerState = true, winStatus = false, timeState = true;
	std::chrono::steady_clock::time_point timePoint, timeBegin;
	SDL_Event event;
	World world = World();
	SoundController soundMixer = SoundController();

	Screen* screen = new Screen();
	screen->setTimeBegin(std::chrono::steady_clock::now());

	KeyboardController controller = KeyboardController();
	const Uint8* state = SDL_GetKeyboardState(NULL);

	loadImages(screen->getDisplay());

	if (!screen->getInitStatus()) {
		Player* player = new Player(new Position(35, 400));
		world.setPlayer(player);
		world.setScreen(screen);
		screen->setPlayer(player);

		while (player->getLives() && !winStatus) {
			resetGame(controller, player, world, screen, &playerState);

			timeBegin = std::chrono::steady_clock::now();
			while (std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - timeBegin).count() <= 3000) {
				screen->drawStartScreen();
				timePoint = std::chrono::steady_clock::now();
			}

			SoundController::stopMusic();
			SoundController::playBackgroudMarioMusic();

			while (playerState && timeState && !winStatus) {
				screen->updateScreen(world);

				while (SDL_PollEvent(&event) && playerState) {
					controller.handleKeysState(state);
					controller.handleKeys(player, world);
					if (player->isDead()) {
						playerState = false;
					}
					controller.clearKeysState();
				}

				if (playerState) {
					controller.handleKeys(player, world);
				}

				if (world.isPlayerFinishingWorld() && !winStatus) {
					SoundController::playFlagDownEffect();
					world.switchOnFlag();
					winStatus = true;
					player->resetSteps();
					
					while (!world.isFlagDown()) {
						world.performActions();
						screen->updateScreen(world);
					}
					player->setFinishingRunParameters();

					screen->drawWorldFinishedScreen(world);
				}

				if (player->isDead()) {
					playerState = false;
				}
				else if (screen->isTimePassed() && !winStatus) {
					timeState = false;
				}
			}

			if (!playerState) {
				screen->drawDeadMario(world);
			}
			else if (!timeState) {
				screen->drawTimeUpScreen();
				SoundController::stopMusic();
				SoundController::playTimePassedMusic();
			}
		}
	}

	timeBegin = std::chrono::steady_clock::now();
	while (std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - timeBegin).count() <= 3000 && !winStatus) {
		screen->drawGameOverScreen();
		timePoint = std::chrono::steady_clock::now();
	}
}
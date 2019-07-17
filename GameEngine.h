#pragma once
#include <QObject>
#include <QDebug>
#include <QTimer>

#include <AbstractUnit.h>
#include <AbstractTiles.h>

class GameEngine : public QObject {
	Q_OBJECT;
	QTimer timer;
	int time;

signals:
	void gameFinished(bool win);//might change due to the number of players > 1
	void unitAttacked(AbstractUnit* Attacker, AbstractUnit* Defender);
	void unitMissed(AbstractUnit* Attacker);
	void unitBuilt(AbstractUnit* Builder, AbstractUnit* Building, AbstractTile* Spot/* int x, int y*/);
	void unitDestroyed(AbstractUnit* Destroyed/*, int x, int y*/);//either (unit) or (x,y)
	void tick();
	void turnFinished();
	void unitCreated(AbstractUnit* Created, AbstractTile* Spot/*int x, int y*/);
	void unitCreatedAt(AbstractUnit* Created, int x, int y);
	void tileCreated(/*AbstractTile* tile, */int x, int y);//the game itself will decide what tile it generates
	void tileOccupied(AbstractTile* tile, AbstractUnit* Occupand/*, int x, int y*/);//either (tile) or (x,y)
	void tileFreed(AbstractTile* tile /*,int x, int y*/);
	//void unitMoved(AbstractUnit* Moved, int exX, int exY, int newX, int newY);
	void unitMoved(AbstractUnit* Moved, AbstractTile* Source, AbstractTile* Destination);
public:
	GameEngine() { time = 0; }

	void dealtAttack(AbstractUnit* Attacker, AbstractUnit* Defender) {
		//random chance
		int random = rand() % 100;
		//this (below) happens IN GAME CLASS!!!
		/*int ch = Defender->getCurrentHealth();
		int cdph = Attacker->getCurrentDamagePerHit();
		Defender->modifyCurrentHealth(ch - cdph);
		ch = Defender->getCurrentHealth();*/

		if (random > 50/*attacker->HitChance*/) {/*50% hit chance*/
			emit unitAttacked(Attacker, Defender);
		}
		else {
			emit unitMissed(Attacker);
		}
	}

	void unitPerished(AbstractUnit* Defender, AbstractTile* tile/*int x, int y*/) {
		emit unitDestroyed(Defender);
		emit tileFreed(tile/*x, y*/);
	}



	void startBuild(AbstractUnit* Builder, AbstractUnit* Building, AbstractTile* BuildSpot) {
		//if this build is ready in the current turn of this player

		//emit unitCreated(Building, BuildSpot); - either this or next emit statement... (idk)
		emit unitBuilt(Builder, Building, BuildSpot);
		emit tileOccupied(BuildSpot, Building);
	}

	void unitProgressed(AbstractUnit* Unit, AbstractTile* Source, AbstractTile* Destination) {
		emit tileFreed(Source);
		emit tileOccupied(Destination, Unit);
		emit unitMoved(Unit, Source, Destination);//i don't know if still necessary
	}

	bool isTurnFinished() {
		return time % 120000==0;//2 minutes/turn
	}

	bool isGameFinished() {
		return false;
		//we should check if there are any units left on map of player/enemy/smbd...
	}


	void loadLevel() {
		int board_w = 40;
		int board_h = 30;
		for (int y = 0; y < board_h; y++) {
			for (int x = 0; x < board_w; x++) {
				emit tileCreated(x, y);
				if (rand() % 1200 > 1000)
					emit unitCreatedAt(new Villager{100,20}, x, y);
			}
		}
	}


	void startGame() {
		loadLevel();
		QObject::connect(&timer, &QTimer::timeout, [&]() {
			emit tick();
			if (isTurnFinished()) {
				timer.stop();
				emit turnFinished();
			}

			if (isGameFinished()) {
				timer.stop();
				emit gameFinished(true);
			}
		});
		//generate timeot signal every ms
		timer.start(1);
	}

};


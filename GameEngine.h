#pragma once
#include <QObject>
#include <QDebug>
#include <QTimer>

//#include "Player.h"

#include "AbstractUnit.h"
#include "AbstractTiles.h"

#include <string>
using std::string;

class GameEngine : public QObject {
	Q_OBJECT;
	QTimer timer;
	int time;

	string winner;

signals:
	void allUnitsGenerated();
	void gameFinished(string name);//might change due to the number of players > 1
	void tick();
	void turnFinished();
	void unitCreated(AbstractUnit* Created, AbstractTile* Spot/*int x, int y*/);
	void unitCreatedAt(AbstractUnit* Created, int x, int y);
	void tileCreated(AbstractTile* tile, int x, int y);//the game itself will decide what tile it generates
	void tileOccupied(AbstractTile* tile, AbstractUnit* Occupand/*, int x, int y*/);//either (tile) or (x,y)

public:
	GameEngine() { time = 0; winner = ""; }

	void forceTurnFinish() {
		time = 0;
		emit turnFinished();
	}

	bool isTurnFinished() {
		return time % 1200==0;//2 minutes/turn
	}

	void foundAWinner(string winner) {
		this->winner = winner;
	}

	bool weHaveAWinner() {
		return this->winner!="";
		//we should check if there are any units left on map of player/enemy/smbd...
	}


	void loadLevel() {
		int board_w = 30;
		int board_h = 20;
		int id = 0;
		for (int y = 0; y < board_h; y++) {
			for (int x = 0; x < board_w; x++) {
				if (abs(19 * x + 29 * y - 551) >= 100) {
					emit tileCreated(new GrassTile{ false }, x, y);
					if (rand() % 100 > 98) {
						emit unitCreatedAt(new Villager{ id,100,20,90 }, x, y);
						id++;
					}
					if (rand() % 100 > 98) {
						emit unitCreatedAt(new Tower{ id,1000,20,75 }, x, y);
						id++;
					}
					
					if (rand() % 100 > 98) {
						emit unitCreatedAt(new HorseArcher{ id,200,40,80 }, x, y);
						id++;
					}
				}
				else {
					emit tileCreated(new WaterTile{ false }, x, y);
					if (rand() % 1200 > 1150) {
						emit unitCreatedAt(new Galleon{ id,200,30,75 }, x, y);
						id++;

						/*
						ONLY THE PLAYER CAN CREATE UNITS NOW !!!
						*/
					}
				}
			}
		}
		
		//int id = 0;
		////emit unitCreatedAt(new TownCenter{ id,500,40,0 }, 1, 1);
		//emit unitCreatedAt(new Villager{ id,100,20,90 }, 2, 3);
		//id++;
		//emit unitCreatedAt(new HorseArcher{ id,200,40,80 }, 1, 5);
		//id++;
		//emit unitCreatedAt(new Villager{ id,100,20,90 }, 0, 3);
		//id++;


		//emit unitCreatedAt(new Villager{ id,100,20,90 }, 27, 16);
		//id++;
		//emit unitCreatedAt(new HorseArcher{ id,200,40,80 }, 28, 14);
		//id++;
		//emit unitCreatedAt(new Villager{ id,100,20,90 }, 29, 16);

		emit allUnitsGenerated();
	}


	void startGame() {
		loadLevel();
		QObject::connect(&timer, &QTimer::timeout, [&]() {
			emit tick();
			time++;
			if (isTurnFinished()) {
				emit turnFinished();
			}

			if (weHaveAWinner()) {
				timer.stop();
				emit gameFinished(winner);
			}
		});
		//generate timeot signal every 100 ms
		timer.start(100);
	}

};


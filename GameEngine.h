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
	QTimer timer;//the game clock
	int time;//the value rerpesenting elapsed time (since the game started).

	string winner;//the name of the player remaining alive on the map

signals:
	/*
	Signal that is emitted when the whole map setup is done, (tiles + starting units)
	so that the game can start.
	(the game will assign created units to players depending on the position on the map:
	the units above the river will be P1 and others P2.)
	*/
	void allUnitsGenerated();
	
	/*
	Signal that is emitted when one of the two players has lost all their units,
	consequently, the other being the winner of the game.
	*/
	void gameFinished(string name);//might change due to the number of players > 1

	/*
	Signal that is emitted every 100 miliseconds, representing the game clock;
	how often the game state is being checked.
	*/
	void tick();

	/*
	Signal that is emitted when every turn is finished, whether the time ran out
	or the player has forced the end of the turn. (The game will change the active player.)
	*/
	void turnFinished();

	/*
	Signal that is emitted every time an AbstractUnit* is created. The signal also mentions
	the coordinates (x,y) of the Tile hosting the Unit.
	*/
	void unitCreatedAt(AbstractUnit* Created, int x, int y);

	/*
	Signal that is emitted every time an AbstractTile* is created. The signal also mentions
	the coordinates (x,y) of the Tile on the map layout.
	*/
	void tileCreated(AbstractTile* tile, int x, int y);//the game itself will decide what tile it generates

public:
	GameEngine() { time = 0; winner = ""; }


	/*
	Method that both sets the time to 0 (so that the next turn to start will have exactly two minutes)
	and emits the turnFinished signal, so that the game will change its state immediately. (player turn swap)
	*/
	void forceTurnFinish() {
		time = 0;
		emit turnFinished();
	}

	/*
	Method that is called every 100 ms (in the main clock function)
	and checks if the maximum time of the current turn has finished.
	*/
	bool isTurnFinished() {
		return time % 1200==0;// 2 minutes/turn
	}

	/*
	Method called ONLY BY THE "GAME" class when the game finished
	so that the NAME OF THE WNNER IS SET within the "GameEngine" class,
	which is later going to trigger the end of the game.
	(call of weHaveAWinner() in clock function will return true.)
	*/
	void foundAWinner(string winner) {
		this->winner = winner;
	}

	/*
	Method that is called every 100 ms (in the main clock function)
	and checks if the private string "winner" is no longer null, which
	means that the game finished and we have a winner.
	*/
	bool weHaveAWinner() {
		return this->winner != "";
	}

	/*
	Method that generates a full grid layout of 30 by 20 tiles (of 50 by 50) on the map.
	The colour of the tiles are randomly generated. The tiles respect the rule that there
	must be a river crossing from SW to NE the map.
	On the other hand, the units are hardcoded in the opposite corners of the map,
	so that each player starts with the same chances of winning the game.
	
	Each player starts with:	-1 Town Center
								-2 Villagers
								-1 Horse Archer
	*/
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


	/*
	Main Clock Loop - this function is the "Heart of the Game", it will constantly
	check the game state every 100 miliseconds, emitting signals accordingly.
	The timer starts when the game is created and everything is loaded and ready to run.
	*/
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


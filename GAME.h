#pragma once
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QGraphicsRectItem>
#include <QMouseEvent>

#include "GameEngine.h"
#include "Player.h"
#include "Map.h"

#include <vector>
using std::vector;


class GAME: public QGraphicsView{
private:
	//matrix vector<pointer to vector<Tiles>>
	//matrix vector<pointer to vector<Units>>
	//the position in the matrix of the tile/unit
	//is the actual position on the map
	//vector<vector< AbstractTile*>> tileMatrix;
	//vector<vector< AbstractUnit*>> unitMatrix;

	//.change of plan: there will be two VECTORS.
	//the vector.size() % number of columns => beginning of a row (=> x)
	//from the beginning we add the y to find the element on (x,y)
	vector< AbstractTile*> tileMatrix;
	vector< AbstractUnit*> unitMatrix;

	int player_turn_count, player_count;

	GameEngine* engine;

	QGraphicsScene* scene;

	vector<Player> players;

	QGraphicsRectItem* leftWall;
	QGraphicsRectItem* rightWall;
	QGraphicsRectItem* topWall;
	QGraphicsRectItem* bottomWall;


	void initEnclosingWals() {
		int wallTickness = 8;
		leftWall = new QGraphicsRectItem(0, 0, wallTickness, height());
		auto br = QBrush(Qt::gray);
		leftWall->setBrush(br);
		scene->addItem(leftWall);
		rightWall = new QGraphicsRectItem(width() - wallTickness - 1, 0, wallTickness, height());
		rightWall->setBrush(br);
		scene->addItem(rightWall);
		topWall = new QGraphicsRectItem(0, 0, width(), wallTickness);
		topWall->setBrush(br);
		scene->addItem(topWall);
		bottomWall = new QGraphicsRectItem(0, height() - 50, width(), wallTickness);
		bottomWall->setBrush(br);
		scene->addItem(bottomWall);

	}

	void createPlayer(string name) {//minimalistic
		players.push_back(Player{ name });
	}


	AbstractTile* getTileAt(int x, int y) {
		return this->tileMatrix.at(30 * y + x);
	}

	void addTile(int x, int y) {
		AbstractTile* tile = new GrassTile{false};
		//also add to class matrix of tiles
		//this->tileMatrix.at(x*30+y) = tile;//30 is the height of the table
		//free space should be initialised already (pre pushed_back elements in constructor)
		//whatever x,y and multiply are bad
		
		
		this->tileMatrix.push_back(tile);

		tile->setPos(x*50, y*50);
		scene->addItem(tile);
	}

	
	void addUnit(AbstractUnit* unit, int x, int y) {
		
		this->unitMatrix.push_back(unit);
		//THIS IS WRONG!!!
		//I MUST INITIALISE ALL THE SPACE IN THE MATRIX WITH NULL/NAN ABSTRACT TILES
		//AND REPLACE THEM WHEN A TILE IS ADDED / WHEN A PLAYER OCCUPIES/FREES THE TILE etc...
		//so that the brush is ok


		unit->setPos(x * 50, y * 50);
		AbstractTile* tile = getTileAt(x, y);
		scene->addItem(unit);
		
	}

	void initSignalSlots() {
		
		//advanceGame invoked every time  
		QObject::connect(engine, &GameEngine::tick, this, &GAME::advanceGame);

		QObject::connect(engine, &GameEngine::tileCreated, [&](int x, int y) {
			addTile(x, y);
		});

		QObject::connect(engine, &GameEngine::unitCreatedAt, [&](AbstractUnit* unit, int x, int y) {
			addUnit(unit, x, y);
		});

		QObject::connect(engine, &GameEngine::gameFinished, [&](bool win) {
			if (win) {
				QMessageBox::information(this, "Info", "You win!!!");
			}
			else {
				QMessageBox::information(this, "Info", "You lose!!!");
			}
		});
	}


	//handle mouse move
	void mouseMoveEvent(QMouseEvent* ev) override {
		//works only if setMouseTracking(true);
		auto x = ev->pos().x();
		//player->setPos(x, player->y());
	}

	void keyPressEvent(QKeyEvent* ev) override {
		if (ev->key() == Qt::Key_Left) {
			//player->moveX(-15);
		}
		else if (ev->key() == Qt::Key_Right) {
			//player->moveX(15);
		}
		else if (ev->key() == Qt::Key_Space) {
			//restart
		}

	}

	void advanceGame() {
		int i;
		for (i = 0; i < player_count; i++) {
			if (player_turn_count % player_count == i) {
				auto playerUnits = this->players.at(i).getAllUnits();
				for (auto& unit : playerUnits) {
					
				}
			}
		}
	}


	void initScene() {
		scene = new QGraphicsScene;
		setScene(scene);

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setFixedSize(800, 600);
		scene->setSceneRect(0, 0, 800, 600);
		setBackgroundBrush(QBrush(QColor(215, 214, 213,127)));

	}

public:
	GAME(GameEngine* engine) :engine{ engine } {
		setMouseTracking(true);
		initScene();
		initEnclosingWals();
		createPlayer("Wallace");

		initSignalSlots();
	}
};


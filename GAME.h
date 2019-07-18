#pragma once
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QGraphicsRectItem>
#include <QMouseEvent>

#include <qlistwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>

#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "AbstractUnit.h"

#include <string>
#include <vector>
using std::string;
using std::vector;
using std::to_string;


class InfoWindow : public QWidget {
private:
	AbstractTile* tile = nullptr;
	AbstractUnit* unit = nullptr;
	int x, y;

	QLabel* labType = new QLabel{ "Type: " };
	QLabel* labOccupied = new QLabel{ "Occupied: " };
	QLabel* labCrossable = new QLabel{ "Crossable: " };
	QListWidget* lst = new QListWidget;

	void guiSetup() {
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdg = new QWidget;
		QVBoxLayout* lv = new QVBoxLayout;
		wdg->setLayout(lv);
		lv->addWidget(labType);
		lv->addWidget(labOccupied);
		lv->addWidget(labCrossable);
		lv->addWidget(lst);

		linit->addWidget(wdg);
	}

	void initSignalSlots(){}

	void initialGUIState() {
		this->labType->setText(this->labType->text() + QString::fromStdString(tile->getType()));

		if (this->tile->isOccupied() == true) {
			this->labOccupied->setText(this->labOccupied->text() + QString::fromStdString("TRUE"));
			//add info about the occupant in the list
			QListWidgetItem* item = new QListWidgetItem;
			string name = "unit @ (" + to_string(x) + "," + to_string(y) + ")";
			item->setText(QString::fromStdString(name));

			//we could map the id so that we get the name of the unit
			QListWidgetItem* itemH = new QListWidgetItem;
			string infoH = "Health: " + to_string(unit->getCurrentHealth()) + "/" + to_string(unit->getBaseHealth());
			itemH->setText(QString::fromStdString(infoH));
			QListWidgetItem* itemD = new QListWidgetItem;
			string infoD = "Damage: " + to_string(unit->getCurrentDamagePerHit()) + "/" + to_string(unit->getBaseDamagePerHit());
			itemD->setText(QString::fromStdString(infoD));
			QListWidgetItem* itemC = new QListWidgetItem; 
			string infoC = "HitChance: " + to_string(unit->getCurrentHitChance()) + "/" + to_string(unit->getBaseHitChance());
			itemC->setText(QString::fromStdString(infoC));
			lst->addItem(item);
			lst->addItem(itemH);
			lst->addItem(itemD);
			lst->addItem(itemC);
		}
		else {
			this->labOccupied->setText(this->labOccupied->text() + QString::fromStdString("FALSE"));
		}


		if (this->tile->isCrossable() == true) {
			this->labCrossable->setText(this->labCrossable->text() + QString::fromStdString("TRUE"));
		}
		else {
			this->labCrossable->setText(this->labCrossable->text() + QString::fromStdString("FALSE"));
		}
	}


public:
	InfoWindow(AbstractTile* tile, AbstractUnit* unit, int x, int y) : tile{ tile }, unit{ unit }, x{ x }, y{ y } {
		this->setAttribute(Qt::WA_DeleteOnClose);
		guiSetup();
		initSignalSlots();
		initialGUIState();
	}
	
};














#include "Map.h"

class GAME: public QGraphicsView{
private:

	Map* map;

	QGraphicsRectItem* SelectedMark = new QGraphicsRectItem;// (to be deleted in the destructor)	!!!!	!!!!	!!!!	!!!!

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
		return this->map->getTileAt(x, y);
	}

	AbstractUnit* getUnitAt(int x, int y) {
		return this->map->getUnitAt(x, y);
	}

	void addTile(AbstractTile* tile, int x, int y) {

		tile->setPos(x * 50, y * 50);
		scene->addItem(tile);

		this->map->addTile(tile, x, y);
	}

	
	void addUnit(AbstractUnit* unit, int x, int y) {
		
		//auto temp = this->unitMatrix.at(30 * y + x);

		unit->setPos(x * 50, y * 50);
		/*AbstractTile* tile = getTileAt(x, y);
		tile->occupy();*/
		scene->addItem(unit);

		/*this->unitMatrix.at(30 * y + x) = unit;
		delete temp;*/
		this->map->addUnit(unit, x, y);
	}

	void changeTurn() {
		QMessageBox::information(this, "Info", "TURN TIME EXPIRED!");
		
		//all actions undergone must be stopped!

		this->map->changeTurn();
	}


	void initSignalSlots() {
		
		//advanceGame invoked every time
		QObject::connect(engine, &GameEngine::turnFinished, this, &GAME::changeTurn);

		QObject::connect(engine, &GameEngine::tick, this, &GAME::advanceGame);

		QObject::connect(engine, &GameEngine::tileCreated, [&](AbstractTile* tile, int x, int y) {
			addTile(tile, x, y);
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


	void mousePressEvent(QMouseEvent* ev) override {
		int x = ev->pos().x();
		int y = ev->pos().y();

		x /= 50;
		y /= 50;

		auto tile = getTileAt(x, y);
		auto unit = getUnitAt(x, y);

		auto localSelectedTile = getTileAt(x, y);
		auto localSelectedUnit = getUnitAt(x, y);

		if (this->map->wasSelectionNULL()) {
			if (this->map->UnitExistsInSelected(localSelectedUnit)) {
				this->map->setSelection(localSelectedTile, localSelectedUnit, x, y);
				this->SelectedMark->setPos(x * 50, y * 50);
				this->scene->addItem(SelectedMark);
			}
			else {
				/*ignore selection*/
			}
		}
		else {
			//see if local selection is in range of action
			//see if the tile supports the action
			//execute action
			if (this->map->isSameSelection(x, y)) {
				//ignore command
				this->scene->removeItem(SelectedMark);
				this->map->deleteSelection();
			}
			else {
				//we will simplify a lot for now:
				if (this->map->getSelectedUnit()->canMove() && !localSelectedTile->isOccupied() && localSelectedTile->isCrossable() && this->map->getSelectedUnit()->getType() == "LAND") {
					this->map->moveAction(localSelectedTile, x, y);
					this->map->getUnitAt(x, y)->setPos(x * 50, y * 50);
					this->map->deleteSelection();
					this->scene->removeItem(SelectedMark);

				}
				else if (this->map->getSelectedUnit()->canMove() && !localSelectedTile->isOccupied() && !localSelectedTile->isCrossable() && this->map->getSelectedUnit()->getType() == "WATER") {
					this->map->moveAction(localSelectedTile, x, y);
					this->map->getUnitAt(x, y)->setPos(x * 50, y * 50);
					this->map->deleteSelection();
					this->scene->removeItem(SelectedMark);
				}
				else {
					QMessageBox::warning(this, "Permission Warning", "YOU CANNOT EXECUTE THAT ACTION!");
					this->map->deleteSelection();
					this->scene->removeItem(SelectedMark);
				}
			}
		}
	}

	void mouseDoubleClickEvent(QMouseEvent* ev) override{
		int x = ev->pos().x();
		int y = ev->pos().y();
		
		x /= 50;
		y /= 50;
		
		auto tile = getTileAt(x, y);
		auto unit = getUnitAt(x, y);

		InfoWindow* wndw = new InfoWindow{ tile, unit, x, y };
		wndw->show();
	}

	//handle mouse move
	void mouseMoveEvent(QMouseEvent* ev) override {
		//works only if setMouseTracking(true);
		

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
		for (i = 0; i < this->map->getPlayerCount(); i++) {/*MIGHT HAVE TO GO... SOME CALCULATIONS COULD BE DONE IN SERVICE...*/
			if (this->map->getPlayerTurnCount() % this->map->getPlayerCount() == i) {
				auto playerUnits = this->players.at(i).getAllUnits();
				for (auto& unit : playerUnits) {
					/*TO BE CONTINUED*/
				}
			}
		}
	}


	void initScene() {
		scene = new QGraphicsScene;
		setScene(scene);

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setFixedSize(1500, 1000);
		scene->setSceneRect(0, 0, 1500, 1000);
		setBackgroundBrush(QBrush(QColor(215, 214, 213,127)));

	}

public:
	GAME(GameEngine* engine, Map* map) :engine{ engine }, map{ map } {
		setMouseTracking(true);
		initScene();
		initEnclosingWals();
		createPlayer("Wallace");

		initSignalSlots();

		this->SelectedMark->setBrush(QBrush(QColor(224, 195, 30, 100)));
		this->SelectedMark->setRect(0, 0, 50, 50);
	}
};


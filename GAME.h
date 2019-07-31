#pragma once
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QGraphicsRectItem>
#include <QMouseEvent>

#include <qlistwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qpushbutton.h>

#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "AbstractUnit.h"
#include "AbstractAction.h"

#include <string>
#include <vector>
using std::string;
using std::vector;
using std::to_string;


#include <qwidget.h>
#include <qformlayout.h>


#include <QtCore>
#include <QtGui>
#include <QtWidgets>


class ActionWindow : public QWidget {
private:
	Map* map = nullptr;
	AbstractTile* tile = nullptr;
	AbstractUnit* unit = nullptr;
	QListWidget* lst = new QListWidget;
	QPushButton* btnSelect = new QPushButton{ "SELECT ACTION" };
	QLabel* labAct = new QLabel{ "ACTIONS THIS UNIT CAN EXECUTE: " };

	void guiSetup() {
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdg = new QWidget;
		QVBoxLayout* lv = new QVBoxLayout;
		wdg->setLayout(lv);
		lv->addWidget(labAct);
		lv->addWidget(lst);
		lv->addWidget(btnSelect);

		linit->addWidget(wdg);
	}

	void initSignalSlots() {

		QObject::connect(btnSelect, &QPushButton::clicked, this, [&]() {
			if (this->lst->selectedItems().isEmpty()) {
				QMessageBox::warning(this, "Warning", "NO ACTION WAS SELECTED");
			}
			else {
				auto item = this->lst->selectedItems().at(0);
				auto action = item->text().toStdString();
				this->map->setSelectedAction(action);
				this->close();
			}
			; });
	}

	void initialGUIState() {//THIS PART MUST BE CHANGED SO THAT IT ALLOWS ABSTRACT ACTIONS TO BE CREATED
		for (string action : this->unit->getActions()) {
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(QString::fromStdString(action));
			lst->addItem(item);
		}
	}

public:
	ActionWindow(Map* map, AbstractTile* tile, AbstractUnit* unit) : map{ map }, tile{ tile }, unit{ unit } {
		this->setAttribute(Qt::WA_DeleteOnClose);
		guiSetup();
		initSignalSlots();
		initialGUIState();
	}
};


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
			string name = unit->getType() + " unit @ (" + to_string(x) + "," + to_string(y) + ")";
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
			QListWidgetItem* itemS = new QListWidgetItem;
			string infoS = "Speed: " + to_string(unit->getSpeed());
			itemS->setText(QString::fromStdString(infoS));
			QListWidgetItem* itemR = new QListWidgetItem;
			string infoR = "Range: " + to_string(unit->getRange());
			itemR->setText(QString::fromStdString(infoR));
			lst->addItem(item);
			lst->addItem(itemH);
			lst->addItem(itemD);
			lst->addItem(itemC);
			lst->addItem(itemS);
			lst->addItem(itemR);
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

















class GAME : public QGraphicsView {
private:

	Map* map;

	QGraphicsRectItem* SelectedMark = new QGraphicsRectItem;

	GameEngine* engine;

	QGraphicsScene* scene;


	QGraphicsRectItem* infoRect;
	QLabel* labPlayer = new QLabel{ "NOW PLAYING: " };
	QLabel* labCredits = new QLabel{ "CREDITS LEFT: " };
	QPushButton* btnEndTurn = new QPushButton{ "END TURN" };
	


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


	void addUnitNoShow(AbstractUnit* unit, int x, int y) {
		unit->setPos(x * 50, y * 50);
		this->map->addUnit(unit, x, y);
	}
	void addUnit(AbstractUnit* unit, int x, int y) {
		unit->setPos(x * 50, y * 50);
		scene->addItem(unit);
		this->map->addUnit(unit, x, y);
	}

	void changeTurn() {
		QMessageBox::information(this, "Info", "YOUR TURN HAS ENDED!");
		this->map->deleteSelection();
		this->scene->removeItem(SelectedMark);

		//all actions undergone must be stopped!

		this->map->changeTurn();

		/*auto player = this->map->getActivePlayer();
		auto str = player->getName();
		labPlayer->setText("NOW PLAYING: " + QString::fromStdString(str));*/
		//division by 0 in map function...( <= called too early)
	}


	void initSignalSlots() {

		QObject::connect(engine, &GameEngine::turnFinished, this, &GAME::changeTurn);

		QObject::connect(engine, &GameEngine::allUnitsGenerated, this, &GAME::InitUnitColours);

		//advanceGame invoked every time
		QObject::connect(engine, &GameEngine::tick, this, &GAME::advanceGame);

		QObject::connect(engine, &GameEngine::tileCreated, [&](AbstractTile* tile, int x, int y) {
			addTile(tile, x, y);
		});

		QObject::connect(engine, &GameEngine::unitCreatedAt, [&](AbstractUnit* unit, int x, int y) {
			//addUnit(unit, x, y);
			addUnitNoShow(unit, x, y);//this does not help that much reducing memory use...
		});

		QObject::connect(engine, &GameEngine::gameFinished, [&](string winner) {

			string str = winner + " has won the game !!!";
			QString qstr = QString::fromStdString(str);
			QMessageBox::information(this, "Info", qstr);
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

		//if (ev->button() == Qt::MiddleButton) {
		//	/*this restricts the game for mouse users only...*/
		//}

		if (ev->button() == Qt::RightButton) {
			/*
			If the user clicks RMB
			THEN...
			*/
			if (!this->map->wasSelectionNULL() && this->map->isSameSelection(x, y) && tile != nullptr && unit != nullptr && unit->getId() != -1) {
				/*
				If there was selected a tile beforehand
				AND
				the clicked tile corresponds to the selected one (stored in memory)
				AND
				the type of tile is an existing one (not EMPTY TILE)
				AND
				there is a player owned unit on the clicked tile
				THEN...
				*/
				ActionWindow* act = new ActionWindow{ this->map,tile,unit };
				act->show();//this window sends into the service the action to be executed by the selected unit
			}
		}
		else if (ev->button() == Qt::LeftButton) {
			/*
			If the user clicks LMB
			THEN...
			*/
			if (this->map->wasSelectionNULL()) {
				/*
				If there was nothing selected before
				THEN...
				*/
				if (this->map->UnitExistsInSelected(localSelectedUnit) && this->map->SelectedUnitBelongsToActivePlayer(x, y)) {
					/*
					if there is an unit on the clicked tile
					AND
					the unit belongs to the currently active player
					THEN...
					*/
					this->map->setSelection(localSelectedTile, localSelectedUnit, x, y);
					this->SelectedMark->setPos(x * 50, y * 50);
					this->scene->addItem(SelectedMark);//the selection is memorised
				}
				//else {
					/*ignore selection*/
				//}
			}
			else {
				/*
				ELSE (it means that there was something selected beforehand
				*/
				if (this->map->isSameSelection(x, y)) {
					/*
					If the selection is identical to the one in memory
					THEN...
					*/
					this->scene->removeItem(SelectedMark);
					this->map->deleteSelection();//reset selection; ignore command
				}
				else {
					/*
					ELSE (it means that the selection is different from the initial one)
					*/

					//if an action was selected, then we create that action and call its execute() method.
					string action = this->map->getSelectedAction();
					if (action == "NONE") {
						/*ignore click as there is no action selected - no purpose*/
					}
					else {
						int sourcex = this->map->getSelectedX();
						int sourcey = this->map->getSelectedY();
						string actName = this->map->getSelectedAction();
						AbstractActionCreator actionCreator = AbstractActionCreator{ this->map,sourcex,sourcey,x,y,actName };
						AbstractAction* act = actionCreator.returnActionCreated();
						try {
							
							if (act->getCurrentCost() > this->map->getCurrentMoneyLeft())//do not have enough credits for this action
								throw ExecutionException{"THE ACTION REQUIRES TOO MANY CREDITS!!!"};

							act->execute();//all the data modified must be transmitted to players(move, destroy)
							
							this->map->deleteSelection();
							this->scene->removeItem(SelectedMark);
							this->map->payActionExecution(act->getCurrentCost());//actually, the base cost is the cost * multiplier...
							/*if (this->map->getCurrentMoneyLeft() <= 0) {
								this->engine->forceTurnFinish();
							}*/
						}
						catch (ExecutionException& ex) {
							QMessageBox::warning(this, "ACTION FAILED!", QString::fromStdString(ex.print()));
						}
						catch (MessageException& ex) {
							QMessageBox::warning(this, "ACTION SUCCEDED!", QString::fromStdString(ex.print()));
							this->map->payActionExecution(act->getCurrentCost());//actually, the base cost is the cost * multiplier...
						}
					}

					//else (if there is no action selected) (this is gonna remove the "control hack")
					//we should just ignore that click, without deleting any previous recorded information.
				}
			}
		}
	}

	void mouseDoubleClickEvent(QMouseEvent* ev) override {
		int x = ev->pos().x();
		int y = ev->pos().y();

		x /= 50;
		y /= 50;

		auto tile = getTileAt(x, y);
		auto unit = getUnitAt(x, y);

		InfoWindow* wndw = new InfoWindow{ tile, unit, x, y };
		wndw->show();
	}

	void keyPressEvent(QKeyEvent* ev) override {
		if (ev->key() == Qt::Key_End) {
			this->engine->forceTurnFinish();
		}
	}

	void advanceGame() {
		int i;
		if (this->map->getCurrentMoneyLeft() <= 0) {
			this->engine->forceTurnFinish();
		}
		for (auto pl : this->map->getAllPlayers()) {
			if (pl->getRemainingUnits() == 0) {
				this->engine->foundAWinner(pl->getName());
			}
		}

		auto player = this->map->getActivePlayer();
		auto str = player->getName();
		labPlayer->setText("NOW PLAYING: " + QString::fromStdString(str));

		if (player->getColor() == "RED") {
			infoRect->setBrush(QBrush(QColor(255, 0, 0, 255)));
		}
		else {
			infoRect->setBrush(QBrush(QColor(0, 0, 255, 255)));
		}

		auto strng = to_string(this->map->getCurrentMoneyLeft());
		labCredits->setText("CREDITS LEFT: " + QString::fromStdString(strng));
		
	}


	void initScene() {
		scene = new QGraphicsScene;
		setScene(scene);

		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//setFixedSize(1500, 1000);
		setFixedSize(1800, 1000);
		//scene->setSceneRect(0, 0, 1500, 1000);
		scene->setSceneRect(0, 0, 1800, 1000);
		setBackgroundBrush(QBrush(QColor(215, 214, 213, 127)));

		infoRect = new QGraphicsRectItem{ 0,0,200,100 };
	
		infoRect->setPos(1550, 250);
		scene->addItem(infoRect);

		

		QWidget* wdgInfo = new QWidget;
		QFormLayout* lay = new QFormLayout;
		wdgInfo->setLayout(lay);

		/*auto player = this->map->getActivePlayer();
		auto str = player->getName();
		labPlayer->setText("NOW PLAYING: " + QString::fromStdString(str));*/

		lay->addRow(labPlayer);
		lay->addRow(labCredits);
		//lay->addRow(btnEndTurn);

		scene->addWidget(wdgInfo);
		wdgInfo->move(1560, 275);

		//QGraphicsView* graph = new QGraphicsView{};
		

		/*
		I NEED TO CREATE SOME COMMANDS ON THE SCREEN
		*/
	}





	void createPlayer(string name, Civilisation* civ, string color) {
		this->map->createPlayer(name, civ, color);
	}


	void InitUnitColours() {
		Civilisation* britons = new Civilisation{ "Britons" };
		Civilisation* romans = new Civilisation{ "Romans" };
		createPlayer("George200150", britons, "BLUE");
		createPlayer("LordOfUltima98", romans, "RED");

		for (int x = 0; x < 30; x++)
			for (int y = 0; y < 20; y++) {
				if (19 * x < -29 * y + 551) {
					
					this->map->getAllPlayers().at(0)->addUnit(this->map->getUnitAt(x, y), x, y);

				}
				else {
					this->map->getAllPlayers().at(1)->addUnit(this->map->getUnitAt(x, y), x, y);
				}
				/*
				AT THE MOMENT, THERE CAN ONLY BE TWO PLAYERS
				*/
			}

		setPlayerColours();
	}

	void setPlayerColours() {
		for (auto& pl : this->map->getAllPlayers()) {
			pl->setColourForUnits();


		}
		int x = 0;
		int y = 0;
		for (auto& pos : this->map->getAllUnits()) {
			this->scene->removeItem(pos);
			for (auto player : this->map->getAllPlayers()) {
				auto unit = player->getUnitAt(x, y);
				if (unit->getId() != -1) {
					this->scene->addItem(unit);
					break;
				}
			}

			x++;
			if (x == 30) {
				x = 0;
				y++;
			}
		}
	}



public:
	GAME(GameEngine* engine, Map* map) :engine{ engine }, map{ map } {
		this->setAttribute(Qt::WA_DeleteOnClose);
		setMouseTracking(true);
		initScene();

		initSignalSlots();

		this->SelectedMark->setBrush(QBrush(QColor(255, 255, 0, 100)));
		
		this->SelectedMark->setRect(0, 0, 50, 50);
	}
	~GAME() {
		if (this->map != nullptr)
			delete map;
		if (this->engine != nullptr)
			delete engine;
		if (this->SelectedMark != nullptr) {
			this->scene->removeItem(SelectedMark);
			delete SelectedMark;
		}

		delete infoRect;
		qDeleteAll(scene->items());
		delete this->scene;
		
	}

};


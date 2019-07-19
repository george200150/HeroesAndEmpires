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

#include <string>
#include <vector>
using std::string;
using std::vector;
using std::to_string;


class ActionWindow : public QWidget {
private:
	Map* map = nullptr;
	AbstractTile* tile = nullptr;
	AbstractUnit* unit = nullptr;
	QListWidget* lst = new QListWidget;
	QPushButton* btnSelect = new QPushButton{ "SELECT ACTION" };

	void guiSetup() {
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdg = new QWidget;
		QVBoxLayout* lv = new QVBoxLayout;
		wdg->setLayout(lv);
		lv->addWidget(new QLabel{"ACTIONS THIS UNIT CAN EXECUTE: "});
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

	void initialGUIState() {
		for (const auto& action : this->unit->getActions()) {
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(QString::fromStdString(action->getName()));
			lst->addItem(item);
		}
	}

public:
	ActionWindow(Map* map, AbstractTile* tile, AbstractUnit* unit) : map{ map }, tile{ tile }, unit{ unit } {
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

















class GAME: public QGraphicsView{
private:

	int playerId;

	Map* map;

	QGraphicsRectItem* SelectedMark = new QGraphicsRectItem;// (to be deleted in the destructor)	!!!!	!!!!	!!!!	!!!!

	GameEngine* engine;

	QGraphicsScene* scene;

	vector<Player> players;

	InfoWindow* wndw = nullptr;

	/*QGraphicsRectItem* leftWall;
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

	}*/

	void createPlayer(string name, Civilisation* civ, string color) {
		players.push_back(Player{ playerId, name, civ, color });
		this->map->incrementPlayerCount();
		this->playerId++;
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

	
	/*
	THIS METHOD IS INVOKED ONLY AFTER THE CONSTRUCTOR CREATES THE GAME AND GAME ENGINE
	AN THE GAME ENGINE CLOCK STARTS TICKING.

	THIS MEANS THAT WE NEED A RELATIVE BIG DELAY TO WAIT FOR ALL THE UNITS TO BE GENERATED
	SO THAT WE CAN WORK WITH THEM CORRECTLY.

	-I NEED A SIGNAL TO NOTIFY US WHEN ALL THE UNITS HAVE BEEN CREATED - 
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/
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
		this->map->deleteSelection();
		this->scene->removeItem(SelectedMark);
		//if (this->wndw != nullptr) {
		//	wndw->close();
		//}//this is gonna crash the programme... I need to see when the window is closed
		//so that I can assign the nullptr value to class attribute wndw...

		//all actions undergone must be stopped!

		this->map->changeTurn();
	}


	void initSignalSlots() {
		
		//advanceGame invoked every time
		QObject::connect(engine, &GameEngine::turnFinished, this, &GAME::changeTurn);

		QObject::connect(engine, &GameEngine::allUnitsGenerated, this, &GAME::InitUnitColours);

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


	//void Tile_Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
	//{
	//	if (event->buttons() & Qt::LeftButton)
	//	{
	//		// ... handle left click here
	//	}
	//	else if (event->buttons() & Qt::RightButton)
	//	{
	//		// ... handle right click here
	//	}
	//}

	/*what I want is this:

	- when left click is pressed, something is selected. (tiles - source, destination)

	- when right click is pressed, a window with possible actions on that tile is presented
	- the user has to choose one of those actions (idk if it interferes with mousepressevent...)

	- when a tile destination is clicked without action, it becomes source tile (if possible)

	- when a tile destination is clicked with action selected, the action is executed.


	I have to clarify some things... like all those if-s and else-s... i must create a better structure.

	*/

	bool SelectedUnitBelongsToActivePlayer(int x, int y) const {//function uses class parameter turn_count.
		for (auto player : this->players) {
			if (player.getPlayerId() == this->map->getPlayerIdToBeActive()  && player.getAllUnits().at(30 * y + x)->getId() != -1) {
				return true;
			}
		}
		return false;
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

		if (ev->button() == Qt::RightButton) {
			if (!this->map->wasSelectionNULL() && this->map->isSameSelection(x, y) && tile != nullptr && unit != nullptr && unit->getId() != -1) {
				ActionWindow* act = new ActionWindow{ this->map,tile,unit };
				act->show();
			}
		}
		else if (ev->button() == Qt::LeftButton) {

			if (this->map->wasSelectionNULL()) {
				if (this->map->UnitExistsInSelected(localSelectedUnit) /*AND IT BELONGS TO THE ACTIVE PLAYER*/ && SelectedUnitBelongsToActivePlayer(x,y)) {
					this->map->setSelection(localSelectedTile, localSelectedUnit, x, y);
					this->SelectedMark->setPos(x * 50, y * 50);
					this->scene->addItem(SelectedMark);
				}
				//else {
					/*ignore selection*/
				//}
			}
			else {
				if (this->map->isSameSelection(x, y)) {
					//ignore command
					this->scene->removeItem(SelectedMark);
					this->map->deleteSelection();
				}
				//else if (!this->map->wasSelectionNULL() && this->map->UnitExistsInSelected(localSelectedUnit) && !SelectedUnitBelongsToActivePlayer(x, y)) {
				//	/*ignore selection if selected enemy unit*/

				//	//UNFORTUNATELY, I THINK I MISSED TO SET SOME ATTRIBUTES TO THE TILES,
				//	//AS THERE ARE PROBLEMS WHEN EXECUTING MOVE ACTION AND SELECTING SAME UNIT...
				//}
				//THERE MUST BE AN "ELSE IF" HERE, BECAUSE... YOU WILL SEE... :) *HACKS ENEMY*
				//still, there are flaws... I MUST repair this... move function is very unusual...
				else {
					string action = this->map->getSelectedAction();
					if (action == "NONE") {
						//renew selection
						this->map->setSelection(localSelectedTile, localSelectedUnit, x, y);
						this->SelectedMark->setPos(x * 50, y * 50);
						this->scene->addItem(SelectedMark);
					}
					else if (action == "MOVE") {
						if (this->map->getSelectedUnit()->canMove() && !localSelectedTile->isOccupied() && localSelectedTile->isCrossable() && this->map->getSelectedUnit()->getType() == "LAND") {
							this->map->moveAction(localSelectedTile, x, y);
							this->map->getUnitAt(x, y)->setPos(x * 50, y * 50);
							this->map->deleteSelection();
							this->scene->removeItem(SelectedMark);
							this->map->payActionExecution(/*the actual action*/);
						}
						else if (this->map->getSelectedUnit()->canMove() && !localSelectedTile->isOccupied() && !localSelectedTile->isCrossable() && this->map->getSelectedUnit()->getType() == "WATER") {
							this->map->moveAction(localSelectedTile, x, y);
							this->map->getUnitAt(x, y)->setPos(x * 50, y * 50);
							this->map->deleteSelection();
							this->scene->removeItem(SelectedMark);
							this->map->payActionExecution(/*the actual action*/);
						}
						else {
							QMessageBox::warning(this, "Permission Warning", "YOU CANNOT EXECUTE THAT ACTION!");
							this->map->deleteSelection();
							this->scene->removeItem(SelectedMark);
						}
					}//end of move
					else if (action == "ATTACK") {
						if (localSelectedTile->isOccupied()) {//also check if not friendly fire
							this->map->attackAction(this->map->getSelectedUnit(), localSelectedUnit);
							if (localSelectedUnit->getCurrentHealth() <= 0) {
								this->map->destroyAction(localSelectedUnit);
							}
							this->map->deleteSelection();
							this->scene->removeItem(SelectedMark);
							this->map->payActionExecution(/*the actual action*/);

							if (this->map->getActionSuccess()) {
								QMessageBox::information(this, "SUCCESS!", "YOU HIT 'EM HARD!");
							}
							else {
								QMessageBox::information(this, "OH NO!", "YOU MISSED!");
							}
						}
					}//end of attack
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

		wndw = new InfoWindow{ tile, unit, x, y };
		wndw->show();
	}

	////handle mouse move
	//void mouseMoveEvent(QMouseEvent* ev) override {
	//	//works only if setMouseTracking(true);
	//	//player->setPos(x, player->y());
	//}

	void keyPressEvent(QKeyEvent* ev) override {
		if (ev->key() == Qt::Key_End) {
			this->engine->forceTurnFinish();
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
		if (this->map->getCurrentMoneyLeft() == 0){
			this->engine->forceTurnFinish();
		}
		//for (i = 0; i < this->map->getPlayerCount(); i++) {/*MIGHT HAVE TO GO... SOME CALCULATIONS COULD BE DONE IN SERVICE...*/
		//	if (this->map->getPlayerTurnCount() % this->map->getPlayerCount() == i) {
		//		auto playerUnits = this->players.at(i).getAllUnits();
		//		for (auto& unit : playerUnits) {
		//			/*TO BE CONTINUED*/
		//		}
		//	}
		//}
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







	/*
	THIS HAS TO BE THE DEFAULT OPTION FOR RENDERING PLAYER UNITS.
	I MUST DELETE ANY OTHER VERSION OF CREATING NONCOLOR ILLUSTRATION OF UNITS.
	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!
	*/
	void InitUnitColours() {
		Civilisation* britons= new Civilisation{ "Britons" };
		Civilisation* romans= new Civilisation{ "Romans" };
		createPlayer("George200150", britons, "BLUE");
		createPlayer("LordOfUltima98", romans, "RED");

		//AbstractUnit* villager = new Villager{ 1,100,20,90 };
		//this->players.at(0).addUnit(villager, 25, 15);
		////this->addUnit(villager, 25, 15);
		//this->engine->unitCreatedAt(new Villager{ 1,100,20,90 }, 10, 10);


		//villager->setPos(25 * 50, 15 * 50);
		//scene->addItem(villager);
		//	!!!!	!!!!	!!!!	!!!!	!!!! VITAL VITAL VITAL (sa apara pe ecran)


		//villager = new Villager{ 2,100,20,90 };
		//this->players.at(0).addUnit(villager, 9, 9);
		//this->addUnit(villager, 9, 9);

		//AbstractUnit* tower = new Tower{ 3,500,20,75 };
		//this->players.at(0).addUnit(tower, 8, 8);
		//this->addUnit(tower, 8, 8);

		for (int x = 0; x < 30; x++)
			for (int y = 0; y < 20; y++) {
				if (19 * x < -29 * y + 551) {
					players.at(0).addUnit(this->map->getUnitAt(x, y), x, y);

				}
				else {
					players.at(1).addUnit(this->map->getUnitAt(x, y), x, y);
				}
			}

		/*AbstractUnit* unit = new HorseArcher{ 4,200,40,80 };
		this->players.at(1).addUnit(unit);
		this->addUnit(unit, 0, 0);
		unit->setPos(0, 0);
		scene->addItem(unit);*/

		setPlayerColours();// WTF AM I SUPPOSED TO DO?!
	}

	void setPlayerColours() {
		for (auto& pl : this->players) {
			pl.setColourForUnits();
			

		}
		int x = 0;
		int y = 0;
		for (auto& pos : this->map->getAllUnits()) {
			this->scene->removeItem(pos);
			for (auto& player : this->players) {
				auto unit = player.getAllUnits().at(30 * y + x);
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
		setMouseTracking(true);
		initScene();
		//initEnclosingWals(); - NO NEED FOR THAT ANYMORE
//		createPlayer("Wallace");

		initSignalSlots();

		//this->SelectedMark->setBrush(QBrush(QColor(224, 195, 30, 100)));
		this->SelectedMark->setBrush(QBrush(QColor(255, 255, 0, 100)));
		this->SelectedMark->setRect(0, 0, 50, 50);

		//Civilisation* britons = new Civilisation{ "Britons" };
		//Civilisation* romans = new Civilisation{ "Romans" };
		//createPlayer("George200150", britons, "BLUE");
		//createPlayer("LordOfUltima98", romans, "RED");
		this->playerId = 0;
	}
};


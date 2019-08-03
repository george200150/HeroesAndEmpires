#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;
#include "Map.h"
#include "AbstractUnit.h"
#include "AbstractTiles.h"


//class AbstractAction{
//protected:
//	string name;
//	int cost;
//public:
//	AbstractAction(string name, int cost) : name{ name }, cost{ cost } {}
//
//	virtual string getName() const {
//		return this->name;
//	}
//
//	virtual int getCost() const {
//		return this->cost;
//	}
//
//	//virtual void execute() = 0;
//
//	virtual ~AbstractAction() = default;
//};


class BuildingException {
private:
	string msg;
public:
	BuildingException(string msg) : msg{ msg } {}
	string print() const {
		return this->msg;
	}
};

class TrainingException {
private:
	string msg;
public:
	TrainingException(string msg) : msg{ msg } {}
	string print() const {
		return this->msg;
	}
};

class ExecutionException {
private:
	string msg;
public:
	ExecutionException(string msg) : msg{ msg } {}
	string print() const {
		return this->msg;
	}
};


class MessageException {
private:
	string msg;
public:
	MessageException(string msg) : msg{ msg } {}
	string print() const {
		return this->msg;
	}
};


class AbstractAction {
protected:

	Map* map = nullptr;

	//i think that name is futile now...
	//string name;//name can be: "MOVE", "ATTACK", "FORTIFY", "BUILD", "REPAIR", "DOCK".
	int baseCost, currentCost;//cost represents the player points used per action. (cost can support modifiers)
	int sourceX, sourceY;//the coordinates on the map for the origin unit executing the action
	int effectX, effectY;//the coordinates on the map for the efect location of the action

public:
	AbstractAction(Map* map, /*string name,*/int baseCost , int sourceX, int sourceY, int effectX, int effectY) : map{ map },
		/*name{ name },*/baseCost{ baseCost }, currentCost{baseCost}, sourceX{ sourceX }, sourceY{ sourceY }, effectX{ effectX }, effectY{ effectY } {}

	virtual AbstractUnit* getSourceUnit() = 0;
	virtual AbstractUnit* getEffectUnit() = 0;
	virtual AbstractTile* getSourceTile() = 0;
	virtual AbstractTile* getEffectTile() = 0;
	virtual vector<int> getSourceCoords() const = 0;
	virtual vector<int> getEffectCoords() const = 0;
	//virtual string getName() const = 0;
	virtual int getBaseCost() const = 0;
	virtual int getCurrentCost() const {
		return this->currentCost;
	}
	virtual void execute() = 0;

	virtual void modifyCurrentCost(int cost) {
		this->currentCost = cost;
	}
	
	virtual ~AbstractAction() = default;

	/*virtual ~AbstractAction() {
		if (map != nullptr)
			delete map;
	};*/
	virtual void setBuilding(AbstractUnit* new_to_be_built) {}
	virtual void setCharacter(AbstractUnit* new_to_be_trained) {}

};




class MoveAction : public AbstractAction {
protected:
public:
	MoveAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {
		int distsance = this->map->computeDistance(sourceX, sourceY, effectX, effectY);
		auto sourceUnit = this->map->getUnitAt(sourceX, sourceY);
		float speed = sourceUnit->getSpeed();
		int cost = ceil(baseCost * distsance / speed);//every added tile changes the price.
		modifyCurrentCost(cost);
	}
	void execute() override {
		//execute the move action from Source to Destination of the selected unit.

		//I MUST CHECK IF THE UNITS DO NOT OVERLAP
		auto destination = this->map->getTileAt(effectX, effectY);
		if (destination->isOccupied()) {
			/* Ignore action! Cannot occupy again! */
			throw ExecutionException{"TILE ALREADY OCCUPIED!!!"};
		}
		else {
			//I MUST CHECK IF THE UNIT TYPE ALLOWS THE MOVE
			auto sourceUnit = this->map->getUnitAt(sourceX, sourceY);
			if (sourceUnit->getType() == "LAND" && destination->getType() == "GRASS" || sourceUnit->getType() == "WATER" && destination->getType() == "WATER") {
				this->map->getUnitAt(sourceX, sourceY)->setPos(effectX * 50, effectY * 50);
				this->map->moveUnitFromTo(sourceX, sourceY, effectX, effectY);
			}
			else {
				/* ignore action */
				throw ExecutionException{"TILE DOES NOT SUPPORT THE MOVE OF THE UNIT!!!"};
			}
		}
	}

	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
};


class AttackAction : public AbstractAction {
protected:
public:
	AttackAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the attack action from Source(Attacker) to Destination(Defender).

		auto Attacker = this->map->getUnitAt(sourceX, sourceY);

		int distance = this->map->computeDistance(sourceX, sourceY, effectX, effectY);
		int range = Attacker->getRange();

		if (range < distance) {
			throw ExecutionException{ "CANNOT ATTACK THAT FAR!!!" };
		}//CANNOT ATTACK THAT FAR!!!
		
		auto Defender = this->map->getUnitAt(effectX, effectY);
		//DEFENDER MUST BE AN EXISTING UNIT!!!
		if (Defender->getId() == -1)
			throw ExecutionException{"YOU CANNOT ATTACK A BLANK TILE!!!"};

		//I could check if Attacker and Defender are not owned by the same player
		auto Aowner = this->map->getOwnerAt(sourceX, sourceY);
		auto Downer = this->map->getOwnerAt(effectX, effectY);

		if (Aowner->getPlayerId() == Downer->getPlayerId()) {
			/*!!!friendly fire!!!*/;
			throw ExecutionException{"FRIENDLY FIRE!!!"};
		}
		else {
			int damage = Attacker->getCurrentDamagePerHit();
			int hitChance = Attacker->getCurrentHitChance();
			int defHP = Defender->getCurrentHealth();

			if (rand() % 100 <= hitChance) {
				Defender->modifyCurrentHealth(defHP - damage);
			}
			else {
				throw MessageException{ "OH NO, YOU MISSED!" };
			}

			//I should also check if the unit is still alive...

			if (Defender->getCurrentHealth() <= 0) {
				this->map->clearUnitAt(effectX, effectY);
				throw MessageException{"YOU KILLED THEM!"};
			}
			else {
				throw MessageException{ "YOU HIT THEM HARD!" };
			}
		}
	}

	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
};





class FortifyAction : public AbstractAction {
protected:
public:
	FortifyAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the fortification action from Source(Unit) to Destination(Building).

		//void fortifyAction(AbstractUnit* Fortificator, AbstractUnit* Fortificated);
	}
	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
};



//there must still be pointer of the building. the building has been created. we must now create it on map
// then we will (SEND SOMEHOW A MESSAGE TO THE "GAME" SO THAT IT CAN RETRIEVE CHANGED INFORMATION AND SHOW
//  THE UNIT ON THE SCREEN. - maybe an exception... that would be ok :)) ...hope so...

class BuildAction : public AbstractAction {
protected:
	AbstractUnit* to_be_built = nullptr;
	/*as the building has already been created,
	WE MUST DESTROY IT IN CASE ACTION FAILS!!!*/
public:
	BuildAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}

	void execute() override {
		//execute the building action from Source(Builder) to Destination(Building).
		if (this->to_be_built == nullptr) {
			throw ExecutionException{ "NO BUILDING WAS SELECTED!" };
		}

		auto unitAt = this->map->getUnitAt(sourceX, sourceY);
		int range = unitAt->getRange();
		int distance = this->map->computeDistance(sourceX, sourceY, effectX, effectY);
		if (distance > range) {
			delete this->to_be_built;
			throw ExecutionException{ "YOU CANNOT BUILD THAT FAR!" };
		}

		auto tile = this->map->getTileAt(effectX, effectY);
		if (tile->isOccupied() || 
			tile->getType() == "WATER" && to_be_built->getType() == "GRASS" || 
			tile->getType() == "GRASS" && to_be_built->getType() == "WATER") {
			delete to_be_built;//memory leaks if not exception safe
			throw ExecutionException{ "THAT TILE DOES NOT ALLOW THE CONSTRUCTION!" };
		}
			
		else {
			tile->occupy();
			/*THE UNIT IS NOT GOING TO BE SHOWN ON MAP :o*/
			to_be_built->setPos(50 * effectX, 50 * effectY);
			this->map->addUnit(to_be_built, effectX, effectY);
			throw BuildingException{ "BUILDING..." };
		}


		//void buildAction(AbstractUnit* Builder, AbstractUnit* Building);
	}
	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
	void setBuilding(AbstractUnit* new_to_be_built) override {
		this->to_be_built = new_to_be_built;
	}
	void setCharacter(AbstractUnit* new_to_be_trained) override {}
};



class TrainAction : public AbstractAction {
protected:
	AbstractUnit* to_be_trained = nullptr;
public:
	TrainAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the attack action from Source(Trainer) to Destination(Trainee).
		
		if (this->to_be_trained == nullptr) {
			throw ExecutionException{ "NO UNIT WAS SELECTED!" };
		}

		if (this->map->isSurroundAllOccupied(sourceX, sourceY)) {
			delete this->to_be_trained;
			throw ExecutionException{ "FREE SURROUNDING AREA FIRST!!!" };
		}

		
		int distance = this->map->computeDistance(sourceX, sourceY, effectX, effectY);
		if (distance > 1) {
			delete this->to_be_trained;
			throw ExecutionException{ "YOU CANNOT BUILD THAT FAR!" };
		}

		auto tile = this->map->getTileAt(effectX, effectY);
		if (tile->isOccupied()) {
			delete this->to_be_trained;
			throw ExecutionException{ "THAT TILE IS OCCUPIED" };
		}
		else {
			if (tile->getType() == "WATER" && to_be_trained->getType() == "LAND" ||
				tile->getType() == "GRASS" && to_be_trained->getType() == "WATER") {
				delete to_be_trained;//memory leaks if not exception safe
				throw ExecutionException{ "THAT TILE DOES NOT ALLOW THE CONSTRUCTION!" };
			}
			else {
				tile->occupy();
				to_be_trained->setPos(50 * effectX, 50 * effectY);
				this->map->addUnit(to_be_trained, effectX, effectY);
				throw TrainingException{ "TRAINING..." };
			}
		}
		//auto unitAt = this->map->getUnitAt(sourceX, sourceY);
		/*IF THE SOURCE BUILDING ALLOWS TRAINING FOR THAT UNIT TYPE, THEN...*/

		//0 <= x < 20
		//0 <= y < 30

		//if (x + 1 < 20 && !this->map->getTileAt(x, y + 1)->isOccupied() && this->map->getTileAt(x + 1, y)->getType() == "GRASS" && this->to_be_trained->getType() == "WATER" ||
		//		this->map->getTileAt(x + 1, y)->getType() == "WATER" && this->to_be_trained->getType() == "LAND"){//S
		//	this->map->getTileAt(x + 1, y)->occupy();
		//	this->map->addUnit(to_be_trained, x + 1, y);
		//}
		//else if (x + 1 < 20 && y + 1 < 30 && !this->map->getTileAt(x + 1, y + 1)->isOccupied()) {//SE
		//	this->map->getTileAt(x + 1, y + 1)->occupy();
		//	this->map->addUnit(to_be_trained, x + 1, y + 1);
		//}
		//else if (y + 1 < 30 && !this->map->getTileAt(x, y + 1)->isOccupied()) {//E
		//	this->map->getTileAt(x, y + 1)->occupy();
		//	this->map->addUnit(to_be_trained, x, y + 1);
		//}
		//else if (x - 1 >= 0 && y + 1 < 30 && !this->map->getTileAt(x - 1, y + 1)->isOccupied()) {//NE
		//	this->map->getTileAt(x - 1, y + 1)->occupy();
		//	this->map->addUnit(to_be_trained, x - 1, y + 1);
		//}
		//else if (x - 1 >= 0 && !this->map->getTileAt(x - 1, y)->isOccupied()) {//N
		//	this->map->getTileAt(x - 1, y)->occupy();
		//	this->map->addUnit(to_be_trained, x - 1, y);
		//}
		//else if (x - 1 >= 0 && y - 1 >= 0 && !this->map->getTileAt(x - 1, y - 1)->isOccupied()) {//NW
		//	this->map->getTileAt(x - 1, y - 1)->occupy();
		//	this->map->addUnit(to_be_trained, x - 1, y - 1);
		//}
		//else if (y - 1 >= 0 && !this->map->getTileAt(x, y - 1)->isOccupied()) {//W
		//	this->map->getTileAt(x, y - 1)->occupy();
		//	this->map->addUnit(to_be_trained, x, y - 1);
		//}
		//else if (x + 1 < 20 && y - 1 >= 0 && !this->map->getTileAt(x + 1, y - 1)->isOccupied()) {//SW
		//	this->map->getTileAt(x + 1, y - 1)->occupy();
		//	this->map->addUnit(to_be_trained, x + 1, y - 1);
		//}
		//else {
		//	delete this->to_be_trained;
		//	throw ExecutionException{ "NOT ENOUGH ROOM AROUND THE BUILDING!!!" };
		//}

		////auto tile = this->map->getTileAt(effectX, effectY);
		////if (tile->isOccupied() ||
		////	tile->getType() == "WATER" && to_be_built->getType() == "GRASS" ||
		////	tile->getType() == "GRASS" && to_be_built->getType() == "WATER") {
		////	delete to_be_built;//memory leaks if not exception safe
		////	throw ExecutionException{ "THAT TILE DOES NOT ALLOW THE CONSTRUCTION!" };
		////}

		////else {
		////	tile->occupy();
		////	/*THE UNIT IS NOT GOING TO BE SHOWN ON MAP :o*/
		////	to_be_built->setPos(50 * effectX, 50 * effectY);
		////	this->map->addUnit(to_be_built, effectX, effectY);
		////	throw BuildingException{ "BUILDING..." };
		////}


		//void buildAction(AbstractUnit* Builder, AbstractUnit* Building);
	}
	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
	void setBuilding(AbstractUnit* new_to_be_built) override {}
	void setCharacter(AbstractUnit* new_to_be_trained) override {
		this->to_be_trained = new_to_be_trained;
	}
};




class RepairAction : public AbstractAction {
protected:
public:
	RepairAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the reparation action from Source(Repairer) to Destination(Building).
		auto unit = this->map->getUnitAt(effectX, effectY);
		if (unit->getSpeed() != 0)
			throw ExecutionException{ "YOU CAN REPAIR ONLY BUILDINGS!" };
		else {
			int BH = unit->getBaseHealth();
			int CH = unit->getCurrentHealth();
			if (BH == CH) {
				throw ExecutionException{ "BUILDING ALREADY AT FULL HEALTH!" };
			}
			else {
				int repaired = CH + BH / 10;
				if (repaired > BH)//try not to make buildings overhealed...
					repaired = BH;
				unit->modifyCurrentHealth(repaired);
				throw MessageException{ "YOU REPAIRED THE BUILDING!" };
			}
		}
		//void repairAction(AbstractUnit* Repairer, AbstractUnit* Building);
	}
	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
};




class DockAction : public AbstractAction {
protected:
public:
	DockAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the attack action from Source(Attacker) to Destination(Defender).

		//dock...somehow
	}
	AbstractUnit* getSourceUnit() override {
		return this->map->getUnitAt(sourceX, sourceY);
	}
	AbstractUnit* getEffectUnit() override {
		return this->map->getUnitAt(effectX, effectY);
	}
	AbstractTile* getSourceTile() override {
		return this->map->getTileAt(sourceX, sourceY);
	}
	AbstractTile* getEffectTile() override {
		return this->map->getTileAt(effectX, effectY);
	}
	vector<int> getSourceCoords() const override {
		vector<int> xy;
		xy.push_back(this->sourceX);
		xy.push_back(this->sourceY);
		return xy;
	}
	vector<int> getEffectCoords() const override {
		vector<int> xy;
		xy.push_back(this->effectX);
		xy.push_back(this->effectY);
		return xy;
	}
	/*string getName() const override  {
		return this->name;
	}*/
	int getBaseCost() const override {
		return this->baseCost;
	}
};


#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qboxlayout.h>
#include <qmessagebox.h>

//this inherits from AbstractUnitChooser
class AbstractUnitChooser : public QWidget {
private:
	virtual void setupGUI() = 0;
	virtual void initSignalSlots() = 0;
	virtual void initialState() = 0;
public:
	virtual void choose() = 0;
};

class CharacterChooser : public AbstractUnitChooser {
private:
	//* action;
	QLabel* labText = new QLabel{ "SELECT A UNIT TO TRAIN: " };
	QPushButton* btnSelect = new QPushButton{ "SELECT" };
	QListWidget* lst = new QListWidget;

	void setupGUI() override {
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdg = new QWidget;
		QVBoxLayout* lv = new QVBoxLayout;
		wdg->setLayout(lv);
		lv->addWidget(labText);
		lv->addWidget(lst);
		lv->addWidget(btnSelect);

		linit->addWidget(wdg);
	}

	void initSignalSlots() override {
		QObject::connect(btnSelect, &QPushButton::clicked, this, &CharacterChooser::choose);
	}

	void initialState() override {
		QListWidgetItem* item1 = new QListWidgetItem;
		item1->setText("TOWER");
		item1->setData(Qt::UserRole, 1);

		QListWidgetItem* item2 = new QListWidgetItem;
		item2->setText("TOWN CENTER");
		item2->setData(Qt::UserRole, 2);

		QListWidgetItem* item3 = new QListWidgetItem;
		item3->setText("DOCK");
		item3->setData(Qt::UserRole, 3);

		QListWidgetItem* item4 = new QListWidgetItem;
		item4->setText("BARRACKS");
		item4->setData(Qt::UserRole, 4);

		this->lst->addItem(item1);
		this->lst->addItem(item2);
		this->lst->addItem(item3);
		this->lst->addItem(item4);
	}

public:
	void choose() override {}
};

//class BuildingChooser : public AbstractUnitChooser {//this gets the id from map. (no static bs)
//private:
//
//	/*BuildAction**/AbstractAction* action;
//	QLabel* labText = new QLabel{ "SELECT A BUILDING TO CONSTRUCT: " };
//	QPushButton* btnSelect = new QPushButton{ "SELECT" };
//	QListWidget* lst = new QListWidget;
//
//
//	void setupGUI(){
//		QVBoxLayout* linit = new QVBoxLayout;
//		this->setLayout(linit);
//
//		QWidget* wdg = new QWidget;
//		QVBoxLayout* lv = new QVBoxLayout;
//		wdg->setLayout(lv);
//		lv->addWidget(labText);
//		lv->addWidget(lst);
//		lv->addWidget(btnSelect);
//
//		linit->addWidget(wdg);
//	}
//
//	void initSignalSlots() {
//		QObject::connect(btnSelect, &QPushButton::clicked, this, &BuildingChooser::choose);
//	}
//
//	void initialState() {
//		QListWidgetItem* item1 = new QListWidgetItem;
//		item1->setText("TOWER");
//		item1->setData(Qt::UserRole, 1);
//
//		QListWidgetItem* item2 = new QListWidgetItem;
//		item2->setText("TOWN CENTER");
//		item2->setData(Qt::UserRole, 2);
//
//		QListWidgetItem* item3 = new QListWidgetItem;
//		item3->setText("DOCK");
//		item3->setData(Qt::UserRole, 3);
//
//		QListWidgetItem* item4 = new QListWidgetItem;
//		item4->setText("BARRACKS");
//		item4->setData(Qt::UserRole, 4);
//
//		this->lst->addItem(item1);
//		this->lst->addItem(item2);
//		this->lst->addItem(item3);
//		this->lst->addItem(item4);
//	}
//public:
//
//	void choose() {
//		if (this->lst->selectedItems().isEmpty()) {
//			QMessageBox::warning(this, "Warning", "YOU MUST SELECT A BUILDING FIRST!");
//		}
//		else {
//			static int ID = 6;
//			auto item = this->lst->selectedItems().at(0);
//			auto id = item->data(Qt::UserRole);
//			if (id == 1) {
//				action->setBuilding(new Tower{ ID,500,20,75 });//Uh Oh! id is unknown...
//				ID++;
//			}
//			else if (id == 2) {
//				action->setBuilding(new TownCenter{ ID,1000,0,100 });
//				ID++;
//			}
//			else if (id == 3) {
//				action->setBuilding(new Dock{ ID,350,0,0 });
//				ID++;
//			}
//			else if (id == 4) {
//				action->setBuilding(new Barracks{ ID,400,0,0 });
//				ID++;
//			}
//
//
//			this->close();
//		}
//	}
//
//
//	BuildingChooser(AbstractAction* action) : action{ action } {
//		this->setAttribute(Qt::WA_DeleteOnClose);
//		setupGUI();
//		initSignalSlots();
//		initialState();
//	}
//};

/*
TO BE REMEMBERED AND LATER FIXED. COSTS MUST BE SOMEHOW ABOUT THE SAME VALUE
*/
class AbstractActionCreator {
private:
	Map* map;
	AbstractAction* act = nullptr;
	int cost;
public:
	AbstractActionCreator(Map* map, int sourceX, int sourceY, int effectX, int effectY, string actionName) :
		map{ map }, cost{ 1 } {
		
		//AbstractUnit* unit = map->getUnitAt(sourceX, sourceY);
		//int multiplier = unit->getMultiplier();
		//this->cost *= multiplier;

		if (actionName == "MOVE") {
			act = new MoveAction{ map ,cost, sourceX, sourceY, effectX, effectY };
		}
		else if (actionName == "ATTACK") {
			act = new AttackAction{ map ,2 * cost, sourceX, sourceY, effectX, effectY };
		}
		else if (actionName == "FORTIFY") {
			act = new FortifyAction{ map ,cost, sourceX, sourceY, effectX, effectY };
		}
		else if (actionName == "BUILD") {
			
			act = new BuildAction{ map ,3 * cost, sourceX, sourceY, effectX, effectY };
			string to_be_built = this->map->getSelectedBuilding();
			int ID = this->map->getGeneratorId();


			if (to_be_built == "TOWER") {
				act->setBuilding(new Tower{ ID,500,20,75 });
				this->map->incrementGeneratorId();
			}
			else if (to_be_built == "TOWN_CENTER") {
				act->setBuilding(new TownCenter{ ID,1000,0,100 });
				this->map->incrementGeneratorId();
			}
			else if (to_be_built == "DOCK") {
				act->setBuilding(new Dock{ ID,350,0,0 });
				this->map->incrementGeneratorId();
			}
			else if (to_be_built == "BARRACKS") {
				act->setBuilding(new Barracks{ ID,400,0,0 });
				this->map->incrementGeneratorId();
			}
		}
		else if (actionName == "TRAIN") {
			act = new TrainAction{ map ,3 * cost, sourceX, sourceY, effectX, effectY };
			string to_be_trained = this->map->getSelectedTrained();
			int ID = this->map->getGeneratorId();


			if (to_be_trained == "VILLAGER") {
				act->setCharacter(new Villager{ ID,100,10,90 });
				this->map->incrementGeneratorId();
			}
			else if (to_be_trained == "GALLEON") {
				act->setCharacter(new Galleon{ ID,200,20,80 });
				this->map->incrementGeneratorId();
			}
			else if (to_be_trained == "HORSE ARCHER") {
				act->setCharacter(new HorseArcher{ ID,300,30,90 });
				this->map->incrementGeneratorId();
			}
			/*else if (to_be_trained == "BARRACKS") {
				act->setBuilding(new Barracks{ ID,400,0,0 });
				this->map->incrementGeneratorId();
			}*/
		}
		else if (actionName == "REPAIR") {
			act = new RepairAction{ map ,2 * cost, sourceX, sourceY, effectX, effectY };
		}
		else if (actionName == "DOCK") {
			act = new DockAction{ map ,cost, sourceX, sourceY, effectX, effectY };
		}
		//else {}
	}


	AbstractAction* returnActionCreated() {
		return this->act;
	}
};
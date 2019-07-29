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
		//execute the attack action from Source(Attacker) to Destination(Defender).

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





class BuildAction : public AbstractAction {
protected:
public:
	BuildAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the attack action from Source(Attacker) to Destination(Defender).

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
};





class RepairAction : public AbstractAction {
protected:
public:
	RepairAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}
	void execute() override {
		//execute the attack action from Source(Attacker) to Destination(Defender).

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


/*
TO BE REMEMBERED AND LATER FIXED. COSTS MUST BE SOMEHOW ABOUT THE SAME VALUE
*/
class AbstractActionCreator {
private:
	AbstractAction* act = nullptr;
	int cost;
public:
	AbstractActionCreator(Map* map, int sourceX, int sourceY, int effectX, int effectY, string actionName) :
		cost{ 1 } {
		
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
			/*Here, there should be an option to select the building to be constructed...*/
			act = new BuildAction{ map ,3 * cost, sourceX, sourceY, effectX, effectY };
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
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

class AbstractAction {
protected:

	Map* map = nullptr;

	//i think that name is futile now...
	//string name;//name can be: "MOVE", "ATTACK", "FORTIFY", "BUILD", "REPAIR", "DOCK".
	int baseCost;//cost represents the player points used per action. (cost can support modifiers)
	int sourceX, sourceY;//the coordinates on the map for the origin unit executing the action
	int effectX, effectY;//the coordinates on the map for the efect location of the action

public:
	AbstractAction(Map* map, /*string name,*/int baseCost , int sourceX, int sourceY, int effectX, int effectY) : map{ map },
		/*name{ name },*/baseCost{ baseCost }, sourceX{ sourceX }, sourceY{ sourceY }, effectX{ effectX }, effectY{ effectY } {}

	virtual AbstractUnit* getSourceUnit() = 0;
	virtual AbstractUnit* getEffectUnit() = 0;
	virtual AbstractTile* getSourceTile() = 0;
	virtual AbstractTile* getEffectTile() = 0;
	virtual vector<int> getSourceCoords() const = 0;
	virtual vector<int> getEffectCoords() const = 0;
	virtual string getName() const = 0;
	virtual int getBaseCost() const = 0;
	virtual void execute() = 0;
	
	virtual ~AbstractAction() = default;
};




class MoveAction : public AbstractAction {
protected:
	
public:
	MoveAction(Map* map, int baseCost, int sourceX, int sourceY, int effectX, int effectY) :
		AbstractAction{ map ,baseCost, sourceX, sourceY, effectX,effectY } {}


	void execute() override {
		//execute the move action from Source to Destination of the selected unit.
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







*/
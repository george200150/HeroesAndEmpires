#pragma once
#include <string>
using std::string;

#include "Civilisation.h"
#include "AbstractUnit.h"

class Player{
private:
	int id;//1,2,...
	string name;
	Civilisation civ;
	vector<AbstractUnit*> units;
public:
	Player(string name) : name{ name }, civ{ Civilisation{"Gaia"} } {}
	Player(string name, Civilisation civ) : name{ name }, civ{ civ } {}


	vector<AbstractUnit*> getAllUnits() {
		return this->units;
	}

	void addUnit(AbstractUnit* unit) {
		this->units.push_back(unit);
	}

	void deleteUnit(AbstractUnit* unit) {
		//delete the unit
	}
};


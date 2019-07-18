#pragma once
#include <string>
using std::string;

#include "Civilisation.h"
#include "AbstractUnit.h"

class Player{
private:
	int id;//1,2,3,4,5,6.
	string name;//!!!UNIQUE!!!
	string color;//blue,red,yellow,green,purple,white.
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
		int i = 0;
		for (auto& u : this->units) {
			if (u->getId() == unit->getId()) {
				auto temp = this->units.at(i);
				this->units.at(i) = new EmptyUnit{ -1,-1,-1,-1,"-1" };
				delete temp;
				break;
			}
			i++;
		}
	}

	void selectUnit(AbstractUnit* unit) const {
		//???
	}
	
	void actionUnit(AbstractUnit* unit) const {
		//???
	}
};


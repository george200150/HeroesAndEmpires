#pragma once
#include <string>
using std::string;

#include "Civilisation.h"
#include "AbstractUnit.h"

class Player{
private:
	int id;//1,2,3,4,5,6.
	string name;//!!!UNIQUE!!!
	string color;//blue,red,yellow,green,purple,white.(gray for GAIA)
	Civilisation* civ;
	vector<AbstractUnit*> units;
	int unitsAlive;
public:
	Player(string name) : name{ name }, civ{ new Civilisation{"Gaia"} } {this->color = "gray"; initialiseFreeSpace(); unitsAlive = 0; }
	Player(string name, Civilisation* civ) : name{ name }, civ{ civ } {this->color = "gray"; initialiseFreeSpace(); unitsAlive = 0; }
	Player(int id, string name, Civilisation* civ, string color) : id{ id }, name{ name }, civ{ civ }, color{ color } { initialiseFreeSpace(); unitsAlive = 0; }

	int getRemainingUnits() const {//theta(n) - inefficient!!!
		/*int count = 0;
		for (const auto& u : this->units) {
			if (u->getId() != -1)
				count++;
		}
		return count;*/
		return this->unitsAlive;//theta(1) - so efficient! :D
	}

	string getName() const {
		return this->name;
	}

	AbstractUnit* getUnitAt(int x, int y) {
		return this->units.at(30 * y + x);
	}


	void setUnitAt(AbstractUnit* unit, int x, int y) {
		this->units.at(30 * y + x) = unit;
		
		if (unit->getId() != -1)
			this->unitsAlive++;
	}

	void resetUnitAt(AbstractUnit* unit, int x, int y) {
		auto temp = this->units.at(30 * y + x);
		this->units.at(30 * y + x) = unit;
		delete temp;
	}

	void initialiseFreeSpace() {
		int id = -1;
		int baseHealth = -1;
		int damagePerHit = -1;
		int hitChance = -1;
		float speed = -1;
		int range = -1;
		string type = "-1";
		for (int i = 0; i < 30; i++)
			for (int j = 0; j < 20; j++) {
				EmptyUnit* unit = new EmptyUnit{ id, baseHealth, damagePerHit, hitChance, type, speed, range };
				this->units.push_back(unit);
			}
	}

	int getPlayerId() const {
		return this->id;
	}

	void setColourForUnits() {
		for (auto& unit : this->units) {
			if (unit->getId() != -1) {
				string EXimage = unit->getPhoto();
				string photo = EXimage + this->color + ".fw.png";
				unit->setBrush(QImage(QString::fromStdString(photo)));
			}
		}
	}

	vector<AbstractUnit*> getAllUnits() {
		return this->units;
	}

	string getColor() const {
		return this->color;
	}

	Civilisation* getCivilisation() {
		return this->civ;
	}

	void addUnit(AbstractUnit* unit, int x, int y) {
		this->units.at(30 * y + x) = unit;
		
		if(unit->getId() != -1)
			this->unitsAlive++;
	}

	void deleteUnit(AbstractUnit* unit) {
		int i = 0;
		for (auto& u : this->units) {
			if (u->getId() == unit->getId()) {
				auto temp = this->units.at(i);
				this->units.at(i) = new EmptyUnit{ -1,-1,-1,-1,"-1",-1,-1 };
				delete temp;
				
				if (unit->getId() != -1)
					this->unitsAlive--;

				break;
			}
			i++;
		}
	}

	/*

	- create 2 players (Red,Blue)
	- make sure there is no friendly fire
	- probably I should try a for(player : GAME->players (vector)
	- units must have an owner now!

	-units from different players will spawn on different banks of the river
	-limit the units

	-add build action (for unit - basic things)
	-add create unit (for building)

	*/

	void selectUnit(AbstractUnit* unit) const {
		//???
	}
	
	void actionUnit(AbstractUnit* unit) const {
		//???
	}
};


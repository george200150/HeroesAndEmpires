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
	~Player() {
		/*for (auto& unit : this->units)
			if(...)
			delete unit;*///memory leaks... lots of it... TODO SOLVE MEMORY LEAKS!!!!!!!!!!!!
		//for some reason, there are many EmptyTiles left if we delete everything in Map...
		//however, there seems to be ok when deleting units in player...
		while (this->units.size() > 0) {
			delete units.at(units.size() - 1);
			units.pop_back();
		}
		
		
		if (this->civ != nullptr)
			delete civ;
	}

	/*
	Method for testing if there are any units left on the map of this player.
	If there are none, the player has perished and the game ends.
	*/
	int getRemainingUnits() const {
		return this->unitsAlive;
	}

	/*
	Method returning the name of the player.
	The name is set when creating the Game.
	*/
	string getName() const {
		return this->name;
	}

	/*
	Method returning this player's unit at (x,y) coordinates,
	whether there is an EmptyUnit or a normal unit there.
	*/
	AbstractUnit* getUnitAt(int x, int y) {
		return this->units.at(30 * y + x);
	}

	/*

	*/
	void setUnitAt(AbstractUnit* unit, int x, int y) {
		//auto temp = this->units.at(30 * y + x);/*PROBABLY - MEMORY LEAKS HERE *///However, I think not, as I remember the unit replaced was used somewhere else..
		this->units.at(30 * y + x) = unit;
		//delete temp;
		
		if (unit->getId() != -1)
			this->unitsAlive++;
	}


	/*

	*/
	void resetUnitAt(AbstractUnit* unit, int x, int y) {
		auto temp = this->units.at(30 * y + x);
		this->units.at(30 * y + x) = unit;
		delete temp;
	}


	/*
	Method called in the constructor of the Player class.
	This inserts in the "units" vector 600 EmptyUnit-s,
	representing the layout of the map, so that we could use
	the same system of accessing units both for Map and Player.
	*/
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

	/*
	Method returning the id of the player. This id is used when the turn is changed and
	the game has to choose the next player to move. As there are only 2 players on the map,
	we use the "current turn number % total of players" operation when deciding the next player.
	*/
	int getPlayerId() const {
		return this->id;
	}

	/*
	function that is called when all units have been generated so that each of them receive the
	correct colour of their owning player.
	For each unit in the matrix (of this player), we get the photo of the unit and add the colour (IN CAPS)
	of the owner + ".fw.png", as this is the type of file used in this project for photo files.
	The photo now is brushed on the map (with the correct colour).
	*/
	void setColourForUnits() {
		for (auto& unit : this->units) {
			if (unit->getId() != -1) {
				string EXimage = unit->getPhoto();
				string photo = EXimage + this->color + ".fw.png";
				unit->setBrush(QImage(QString::fromStdString(photo)));
			}
		}
	}

	/*
	time consuming function - this must go!
	*/
	vector<AbstractUnit*> getAllUnits() {
		return this->units;
	}

	/*
	Method returning the colour of this player.
	This is used when combining the base photo name of a unit with the colour,
	in order to correctly brush it on the map.
	*/
	string getColor() const {
		return this->color;
	}

	/*
	Method returning the civilisation of this player. (Useless at the moment...)
	*/
	Civilisation* getCivilisation() {
		return this->civ;
	}

	/*
	Method overwriting the unit at (x,y) on the unit matrix.

	---(!!! IF USED INCORRECTLY, THIS WILL CAUSE IRREVERSIBLE MEMORY LEAKS !!!)---

	*/
	void forceAddUnit(AbstractUnit* unit, int x, int y) {
		this->units.at(30 * y + x) = unit;
	}

	/*
	Method that replaces the unit at (x,y) by
	deleting the previous unit, then writing the new info.
	(Also, if the unit added was not an EmptyUnit, the number of alive units increases.
	
	CAUTION: THIS FUNCTION WILL NOT DECREASE THE UNIT COUNT WHEN DELETING THE PREVIOUS UNIT.
	WE ASSUME THAT THE UNIT DELETED -WAS NOT- A NORMAL UNIT.)
	*/
	void addUnit(AbstractUnit* unit, int x, int y) {
		auto temp = this->units.at(30 * y + x);
		this->units.at(30 * y + x) = unit;/*(used to be) MEMORY LEAK HERE!!!!!!!!!!!*/
		delete temp;
		
		if(unit->getId() != -1)
			this->unitsAlive++;
	}


	/*
	Method that searches an unit in the matrix (using its id) and deletes it, replacing
	the position on map with an EmptyUnit. If the deleted unit was a normal one, the alive
	unit count decreases.

	WARNING: this function is time consuming, complexity = O(n), where n = 30x20.
	( We could have the alternative faster function - Theta(1), namely e.g. DeleteUnitAt() )
	*/
	void deleteUnit(AbstractUnit* unit) {
		int i = 0;
		for (auto& u : this->units) {
			if (u->getId() == unit->getId()) {
				
				if (unit->getId() != -1)
					this->unitsAlive--;

				auto temp = this->units.at(i);
				this->units.at(i) = new EmptyUnit{ -1,-1,-1,-1,"-1",-1,-1 };
				delete temp;

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

};


#pragma once


#include "AbstractTiles.h"
#include "AbstractUnit.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

/*
This class is the actual service of the GAME
*/
class Map {
private:
	//.change of plan: there will be two VECTORS.
	//the vector.size() % number of columns => beginning of a row (=> x)
	//from the beginning we add the y to find the element on (x,y)
	vector< AbstractTile*> tileMatrix;
	vector< AbstractUnit*> unitMatrix;

	int player_turn_count, player_count;
	int base_money_per_turn;
	string selectedAction;

	AbstractTile* SelectedTile = nullptr;
	AbstractUnit* SelectedUnit = nullptr;
	int selectedX = -1, selectedY = -1;
	bool actionSucceded = false;

public:

	vector<AbstractUnit*> getAllUnits() {
		return this->unitMatrix;
	}

	vector<int> getXYbyUnit(AbstractUnit* unit) const {
		vector<int> xy;
		int x = 0;
		int y = 0;
		for (const auto& u : this->unitMatrix) {
			if (unit->getId() == u->getId()) {
				
				xy.push_back(x);
				xy.push_back(y);
				break;
			}
			x++;
			if (x >= 30) {//width of matrix reached
				y++;
				x = 0;
			}
		}
		return xy;
	}

	Map() { initialiseFreeSpace(); this->selectedAction = "NONE"; }

	void setSelectedAction(string action) {
		this->selectedAction = action;
	}

	string getSelectedAction() {
		return this->selectedAction;
	}


	bool wasSelectionNULL() {
		return this->SelectedTile == nullptr && this->SelectedUnit == nullptr;
	}

	bool isSameSelection(int x, int y) {
		return this->selectedX == x && this->selectedY == y;
	}

	void deleteSelection() {
		this->SelectedTile = nullptr;
		this->SelectedUnit = nullptr;
		this->selectedX = -1;
		this->selectedY = -1;
		this->selectedAction = "NONE";
	}

	bool UnitExistsInSelected(AbstractUnit* localSelectedUnit) {
		return localSelectedUnit->getId() != -1;
	}

	void setSelection(AbstractTile* localSelectedTile, AbstractUnit* localSelectedUnit, int x, int y) {
		this->SelectedTile = localSelectedTile;
		this->SelectedUnit = localSelectedUnit;
		this->selectedX = x;
		this->selectedY = y;
		//this->SelectedMark->setPos(x * 50, y * 50);
		//this->scene->addItem(SelectedMark);
	}

	int getPlayerTurnCount() const {
		return this->player_turn_count;
	}
	int getPlayerCount() const {
		return this->player_count;
	}
	int getBaseMoneyPerTurn() const {
		return this->base_money_per_turn;
	}


	int getSelectedX() const {
		return this->selectedX;
	}
	int getSelectedY() const {
		return this->selectedY;
	}
	void setSelectedX(int newX) {
		this->selectedX = newX;
	}
	void setSelectedY(int newY) {
		this->selectedY = newY;
	}

	AbstractTile* getSelectedTile() {
		return this->SelectedTile;
	}
	AbstractUnit* getSelectedUnit() {
		return this->SelectedUnit;
	}
	


	AbstractTile* getTileAt(int x, int y) {
		return this->tileMatrix.at(30 * y + x);
	}

	AbstractUnit* getUnitAt(int x, int y) {
		return this->unitMatrix.at(30 * y + x);
	}

	void addTile(AbstractTile* tile, int x, int y) {
		auto temp = this->tileMatrix.at(30 * y + x);

		tile->setPos(x * 50, y * 50);
		//scene->addItem(tile);/*in GAME*/

		this->tileMatrix.at(30 * y + x) = tile;
		delete temp;
	}


	void addUnit(AbstractUnit* unit, int x, int y) {

		auto temp = this->unitMatrix.at(30 * y + x);

		unit->setPos(x * 50, y * 50);
		AbstractTile* tile = getTileAt(x, y);
		tile->occupy();
		//scene->addItem(unit);/*in GAME*/

		this->unitMatrix.at(30 * y + x) = unit;
		delete temp;
	}

	void changeTurn() {
		//QMessageBox::information(this, "Info", "TURN TIME EXPIRED!");/*in GAME*/

		//all actions undergone must be stopped!

		player_turn_count++;
		base_money_per_turn = 1 + player_count / player_count;
	}


	void initialiseFreeSpace() {
		int id = -1;
		int baseHealth = -1;
		int damagePerHit = -1;
		int hitChance = -1;
		string type = "-1";
		for (int i = 0; i < 30; i++)
			for (int j = 0; j < 20; j++) {
				EmptyUnit* unit = new EmptyUnit{ id, baseHealth, damagePerHit, hitChance,type };
				this->unitMatrix.push_back(unit);
				EmptyTile* tile = new EmptyTile{ false,false };
				this->tileMatrix.push_back(tile);
			}
	}

	//we already know the source and the unit at this point
	void moveAction(/*AbstractTile* Source, */AbstractTile* Destination, /*AbstractUnit* Unit,*/ int destX, int destY) {
		auto exUnit = getUnitAt(selectedX, selectedY);//get unit @ source
		this->unitMatrix.at(30 * destY + destX) = exUnit;//move the pointer to the unit to another tile
		this->unitMatrix.at(30 * selectedY + selectedX) = new EmptyUnit{ -1,-1,-1,-1,"-1" };//mark the last position of the unis as free
		this->SelectedTile->unoccupy();
		Destination->occupy();
	}

	bool getActionSuccess() const {
		return this->actionSucceded;
	}

	void attackAction(AbstractUnit* Attacker, AbstractUnit* Defender) {
		int health = Defender->getCurrentHealth();
		int hit = Attacker->getCurrentDamagePerHit();
		if (rand() % 100 <= Attacker->getCurrentHitChance()) {
			Defender->modifyCurrentHealth(health - hit);
		}
		if (health == Defender->getCurrentHealth())
			this->actionSucceded = false;
		else {
			this->actionSucceded = true;
		}
	}
	void fortifyAction(AbstractUnit* Fortificator, AbstractUnit* Fortificated);
	void buildAction(AbstractUnit* Builder, AbstractUnit* Building);
	void repairAction(AbstractUnit* Repairer, AbstractUnit* Building);
	void destroyAction(AbstractUnit* Destroyed) {
		//this->player_count->deleteUnit();
		auto temp = Destroyed;
		vector<int> xy = getXYbyUnit(Destroyed);
		Destroyed = new EmptyUnit{ -1,-1,-1,-1,"-1" };//this could go
		delete temp;//nu ii place aici ca se sterge...
		//ALSO MODIFY IN 
		int x = xy.at(0);
		int y = xy.at(1);
		this->unitMatrix.at(30 * y + x) = new EmptyUnit{ -1,-1,-1,-1,"-1" };
		this->tileMatrix.at(30 * y + x)->unoccupy();
	}
};


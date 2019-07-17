#pragma once
#include <string>
using std::string;

class AbstractTile{
protected:
	bool crossable;
	bool occupied;
	string image;
	//vector<Player> explored; - - -  maybe a string is enough *map<string,bool> explored* - the string uniquely identifies the player (id)
public:
	AbstractTile(bool crossable, bool occupied) :crossable{ crossable }, occupied{ occupied } {this->image = "NULL"; }

	virtual string getType() const = 0;

	virtual ~AbstractTile() = default;
};

class GrassTile : public AbstractTile {
protected:

public:
	GrassTile(bool occupied) : AbstractTile{ true, occupied } {}

	string getType() const override {//this could be useful when deciding what unit can cross a tile (water units vs land units)
		return "GRASS";
	}

	bool isCrossable() const {
		return this->crossable;
	}

	bool isOccupied() const {
		return this->occupied;
	}

	void occupy() {
		this->occupied = true;
	}

	void unoccupy() {
		this->occupied = false;
	}

	//maybe also set crossable... there may be bridges that could make water crossable... and the overwrite of the uncrossable path could be done incorrectly resulting in uncrossable bridges.
};
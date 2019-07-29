#pragma once
#include <QtWidgets/QGraphicsRectItem>
#include <QBrush>
#include <string>
using std::string;


/*
Class representing a 50 by 50 square on the map.
This is one of the tiles belonging to a the game.
The tile has properties if it is crossable or occupied.
*/
class AbstractTile : public QGraphicsRectItem {
protected:
	bool crossable;//if player units can stand on it
	bool occupied;//if there is an unit on the tile
	string image;// this is kinda useless...
	//vector<Player> explored; - - -  maybe a string is enough *map<string,bool> explored* - the string uniquely identifies the player (id)
public:

	/*
	Constructor of the AbstractTile class. This method sets the crossable and occupied attributes.
	Also, it automatically sets the shape of the unit to a 50x50 square.
	*/
	AbstractTile(bool crossable, bool occupied) :crossable{ crossable }, occupied{ occupied } {
		this->image = "NULL";
		setRect(0, 0, 50, 50);
	}



	/*
	Method that returns the type of the tile.
	The type is whether "GRASS", "WATER" or "EMPTY".
	The attribute type is "EMPTY" when the map is being created, and the vectors of units and tiles
	are empty. We need to do this in order to access any element of the so-called "matrix".
	When accessing an element @ [i][j] we call vector.at(width * y + x) we need to have an existing value,
	else memory access violation happens.
	*/
	virtual string getType() const = 0;



	/*
	Method that returns whether the tile can be crossed by LAND UNITS or not.
	This value is used when allowing LAND and WATER units to advance on the map.
	*/
	virtual bool isCrossable() const = 0;



	/*
	Method that returns whether the tile is occupied or not.
	This value is used when deciding if a tile can be occupied by an unit or not.
	*/
	virtual bool isOccupied() const = 0;


	
	/*
	Method that marks the tile as occupied
	PRECONDITION: the tile MUST NOT be occupied already.
	*/
	virtual void occupy() = 0;



	/*
	Method that marks the tile as free
	PRECONDITION: the tile MUST be occupied already.
	*/
	virtual void unoccupy() = 0;

	virtual ~AbstractTile() = default;
};

class EmptyTile : public AbstractTile {
protected:
public:
	EmptyTile(bool crossable, bool occupied) : AbstractTile{ crossable,occupied } {
		this->image = "NULL";
		setRect(0, 0, 50, 50);
	}

	virtual string getType() const { return "EMPTY"; };
	virtual bool isCrossable() const { return false; }
	virtual bool isOccupied() const { return false; }
	virtual void occupy() {}
	virtual void unoccupy() {}

};

class GrassTile : public AbstractTile {
protected:

public:
	GrassTile(bool occupied) : AbstractTile{ true, occupied } {
		int option = rand();
		if (option % 3 == 0) {
			setBrush(QBrush(QColor(129, 240, 181, 100)));
			//setBrush(QBrush(QColor(50, 137, 48, 255)));
		}
		else if (option % 3 == 1) {
			setBrush(QBrush(QColor(122, 240, 146, 100)));
			//setBrush(QBrush(QColor(128, 195, 66, 255)));
		}
		else {
			setBrush(QBrush(QColor(70, 219, 122, 100)));
			//setBrush(QBrush(QColor(0, 99, 37, 255)));
		}


	}

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


class WaterTile : public AbstractTile {
protected:

public:
	WaterTile(bool occupied) : AbstractTile{ false, occupied } {
		int option = rand();
		if (option % 2 == 0) {
			setBrush(QBrush(QColor(109, 197, 237, 100)));
			//setBrush(QBrush(QColor(20, 20, 140, 255)));
		}
		else {
			setBrush(QBrush(QColor(109, 171, 237, 100)));
			//setBrush(QBrush(QColor(20, 170, 255, 255)));
		}
	}

	string getType() const override {//this could be useful when deciding what unit can cross a tile (water units vs land units)
		return "WATER";
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
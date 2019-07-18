#pragma once
#include <QtWidgets/QGraphicsRectItem>
#include <QBrush>
#include <string>
using std::string;

class AbstractTile : public QGraphicsRectItem {
protected:
	bool crossable;
	bool occupied;
	string image;
	//vector<Player> explored; - - -  maybe a string is enough *map<string,bool> explored* - the string uniquely identifies the player (id)
public:
	AbstractTile(bool crossable, bool occupied) :crossable{ crossable }, occupied{ occupied } {
		this->image = "NULL";
		setRect(0, 0, 50, 50);
	}

	virtual string getType() const = 0;
	virtual bool isCrossable() const = 0;
	virtual bool isOccupied() const = 0;
	virtual void occupy() = 0;
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
			setBrush(QBrush(QColor(50, 137, 48, 255)));
		}
		else if (option % 3 == 1) {
			setBrush(QBrush(QColor(128, 195, 66, 255)));
		}
		else {
			setBrush(QBrush(QColor(0, 99, 37, 255)));
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
			setBrush(QBrush(QColor(20, 20, 140, 255)));
		}
		else {
			setBrush(QBrush(QColor(20, 170, 255, 255)));
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
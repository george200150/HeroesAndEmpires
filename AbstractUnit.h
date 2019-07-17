#pragma once
#include <QGraphicsRectItem>
#include <QBrush>
#include <string>
using std::string;

//#include "Player.h"


class AbstractUnit : public QGraphicsRectItem {
protected:
	int id;
	int baseHealth;
	int currentHealth;
	int BaseDamagePerHit;
	int currentDamagePerHit;
	//Player* owner;

	//vector<Action*> actions; - this is gonna be my next headache for this week
public:
	AbstractUnit(int id, int baseHealth, int damagePerHit) : id{ id }, baseHealth { baseHealth }, BaseDamagePerHit{ damagePerHit }{
		setRect(0, 0, 50, 50);
		currentHealth = baseHealth;
		currentDamagePerHit = damagePerHit;
	}

	//virtual Player* getOwner() = 0;	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!
	virtual int getId() const = 0;
	virtual int getBaseHealth() const = 0;
	virtual void modifyBaseHealth(int newBH) = 0;
	virtual int getCurrentHealth() const = 0;
	virtual void modifyCurrentHealth(int newCH) = 0;
	virtual int getBaseDamagePerHit() const = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) = 0;
	virtual int getCurrentDamagePerHit() const = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) = 0;

	virtual ~AbstractUnit() = default;
};



class AbstractCharacter : public AbstractUnit {
protected:
	//int speed; - this gonna be my first headache for today
	//int range;
public:
	AbstractCharacter(int id, int baseHealth, int damagePerHit) : AbstractUnit{ id, baseHealth ,damagePerHit } {
	}

	virtual int getId() const override = 0;
	virtual int getBaseHealth() const override = 0;
	virtual void modifyBaseHealth(int newBH) override = 0;
	virtual int getCurrentHealth() const override = 0;
	virtual void modifyCurrentHealth(int newCH) override = 0;
	virtual int getBaseDamagePerHit() const override = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) override = 0;
	virtual int getCurrentDamagePerHit() const override = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) override = 0;

	virtual ~AbstractCharacter() = default;
};


class AbstractBuilding : public AbstractUnit {
protected:
	//int range;
public:
	AbstractBuilding(int id, int baseHealth, int damagePerHit) : AbstractUnit{ id, baseHealth ,damagePerHit } {
	}

	virtual int getId() const override= 0;
	virtual int getBaseHealth() const override = 0;
	virtual void modifyBaseHealth(int newBH) override = 0;
	virtual int getCurrentHealth() const override = 0;
	virtual void modifyCurrentHealth(int newCH) override = 0;
	virtual int getBaseDamagePerHit() const override = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) override = 0;
	virtual int getCurrentDamagePerHit() const override = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) override = 0;

	virtual ~AbstractBuilding() = default;
};


class Empty : public AbstractUnit {
protected:
public:
	Empty(int id, int baseHealth, int damagePerHit) : AbstractUnit{ id, baseHealth ,damagePerHit } {
	}

	virtual int getId() const override { return -1; }
	virtual int getBaseHealth() const override { return -1; }
	virtual void modifyBaseHealth(int newBH) override {}
	virtual int getCurrentHealth() const override { return -1; }
	virtual void modifyCurrentHealth(int newCH) override {}
	virtual int getBaseDamagePerHit() const override { return -1; }
	virtual void modifyBaseDamagePerHit(int newBDPH) override {}
	virtual int getCurrentDamagePerHit() const override { return -1; }
	virtual void modifyCurrentDamagePerHit(int newCDPH) override {}
};

class Villager : public AbstractCharacter {
protected:
	string image;
public:
	Villager(int id, int baseHealth, int damagePerHit) : AbstractCharacter{ id, baseHealth ,damagePerHit } {
		image = "NULL";
		setBrush(QBrush(QImage("villager.fw.png")));
	}

	string getImage() const {
		return this->image;
	}

	int getId() const override {
		return this->id;
	}

	int getBaseHealth() const override {
		return this->baseHealth;
	}

	void modifyBaseHealth(int newBH)  override {
		this->baseHealth = newBH;
	}


	int getCurrentHealth() const  override {
		return this->currentHealth;
	}

	void modifyCurrentHealth(int newCH)  override {
		this->currentHealth = newCH;
	}


	int getBaseDamagePerHit() const  override {
		return this->BaseDamagePerHit;
	}

	void modifyBaseDamagePerHit(int newBDPH)  override {
		this->BaseDamagePerHit = newBDPH;
	}


	int getCurrentDamagePerHit() const  override {
		return this->currentDamagePerHit;
	}

	void modifyCurrentDamagePerHit(int newCDPH)  override {
		this->currentDamagePerHit = newCDPH;
	}
};




class Tower : public AbstractBuilding {
protected:
	string image;
public:
	Tower(int id, int baseHealth, int damagePerHit) : AbstractBuilding{ id, baseHealth ,damagePerHit } {
		image = "NULL";
		setBrush(QBrush(QImage("tower.fw.png")));
	}

	string getImage() const {
		return this->image;
	}

	int getId() const override {
		return this->id;
	}

	int getBaseHealth() const  override {
		return this->baseHealth;
	}

	void modifyBaseHealth(int newBH)  override {
		this->baseHealth = newBH;
	}


	int getCurrentHealth() const  override {
		return this->currentHealth;
	}

	void modifyCurrentHealth(int newCH)  override {
		this->currentHealth = newCH;
	}


	int getBaseDamagePerHit() const  override {
		return this->BaseDamagePerHit;
	}

	void modifyBaseDamagePerHit(int newBDPH)  override {
		this->BaseDamagePerHit = newBDPH;
	}


	int getCurrentDamagePerHit() const  override {
		return this->currentDamagePerHit;
	}

	void modifyCurrentDamagePerHit(int newCDPH)  override {
		this->currentDamagePerHit = newCDPH;
	}
};
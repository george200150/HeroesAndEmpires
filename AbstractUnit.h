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
	int baseHitChance;
	int currentHitChance;
	string type;
	//Player* owner;

	//vector<Action*> actions; - this is gonna be my next headache for this week
public:
	AbstractUnit(int id, int baseHealth, int damagePerHit, int hitChance, string type) : id{ id }, baseHealth{ baseHealth }, BaseDamagePerHit{ damagePerHit }, baseHitChance{ hitChance }, type{ type } {
		setRect(0, 0, 50, 50);
		currentHealth = baseHealth;
		currentDamagePerHit = damagePerHit;
		currentHitChance = hitChance;
	}

	//virtual Player* getOwner() = 0;	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!	!!!!
	virtual bool canMove() const = 0;
	virtual string getType() const = 0;
	virtual int getId() const = 0;
	virtual int getBaseHealth() const = 0;
	virtual void modifyBaseHealth(int newBH) = 0;
	virtual int getCurrentHealth() const = 0;
	virtual void modifyCurrentHealth(int newCH) = 0;
	virtual int getBaseDamagePerHit() const = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) = 0;
	virtual int getCurrentDamagePerHit() const = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) = 0;
	virtual int getBaseHitChance() const = 0;
	virtual void modifyBaseHitChance(int newBHC) = 0;
	virtual int getCurrentHitChance() const = 0;
	virtual void modifyCurrentHitChance(int newCHC) = 0;

	virtual ~AbstractUnit() = default;
};



class AbstractCharacter : public AbstractUnit {
protected:
	//int speed; - this gonna be my first headache for today
	//int range;
public:
	AbstractCharacter(int id, int baseHealth, int damagePerHit, int hitChange, string type) : AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type } {
	}

	virtual bool canMove() const override = 0;
	virtual string getType() const override = 0;
	virtual int getId() const override = 0;
	virtual int getBaseHealth() const override = 0;
	virtual void modifyBaseHealth(int newBH) override = 0;
	virtual int getCurrentHealth() const override = 0;
	virtual void modifyCurrentHealth(int newCH) override = 0;
	virtual int getBaseDamagePerHit() const override = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) override = 0;
	virtual int getCurrentDamagePerHit() const override = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) override = 0;
	virtual int getBaseHitChance() const override = 0;
	virtual void modifyBaseHitChance(int newBHC) override = 0;
	virtual int getCurrentHitChance() const override = 0;
	virtual void modifyCurrentHitChance(int newCHC) override = 0;

	virtual ~AbstractCharacter() = default;
};


class AbstractBuilding : public AbstractUnit {
protected:
	//int range;
public:
	AbstractBuilding(int id, int baseHealth, int damagePerHit, int hitChange, string type) : AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type } {
	}

	virtual bool canMove() const override = 0;
	virtual string getType() const override = 0;
	virtual int getId() const override= 0;
	virtual int getBaseHealth() const override = 0;
	virtual void modifyBaseHealth(int newBH) override = 0;
	virtual int getCurrentHealth() const override = 0;
	virtual void modifyCurrentHealth(int newCH) override = 0;
	virtual int getBaseDamagePerHit() const override = 0;
	virtual void modifyBaseDamagePerHit(int newBDPH) override = 0;
	virtual int getCurrentDamagePerHit() const override = 0;
	virtual void modifyCurrentDamagePerHit(int newCDPH) override = 0;
	virtual int getBaseHitChance() const override = 0;
	virtual void modifyBaseHitChance(int newBHC) override = 0;
	virtual int getCurrentHitChance() const override = 0;
	virtual void modifyCurrentHitChance(int newCHC) override = 0;

	virtual ~AbstractBuilding() = default;
};


class EmptyUnit : public AbstractUnit {
protected:
public:
	EmptyUnit(int id, int baseHealth, int damagePerHit, int hitChange, string type) : AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type } {
	}

	virtual bool canMove() const override { return false; }
	virtual string getType() const override { return "-1"; }
	virtual int getId() const override { return -1; }
	virtual int getBaseHealth() const override { return -1; }
	virtual void modifyBaseHealth(int newBH) override {}
	virtual int getCurrentHealth() const override { return -1; }
	virtual void modifyCurrentHealth(int newCH) override {}
	virtual int getBaseDamagePerHit() const override { return -1; }
	virtual void modifyBaseDamagePerHit(int newBDPH) override {}
	virtual int getCurrentDamagePerHit() const override { return -1; }
	virtual void modifyCurrentDamagePerHit(int newCDPH) override {}
	virtual int getBaseHitChance() const override { return -1; }
	virtual void modifyBaseHitChance(int newBHC) override {}
	virtual int getCurrentHitChance() const override { return -1; }
	virtual void modifyCurrentHitChance(int newCHC) override {}
};

class Villager : public AbstractCharacter {
protected:
public:
	Villager(int id, int baseHealth, int damagePerHit, int hitChange) : AbstractCharacter{ id, baseHealth ,damagePerHit, hitChange, "LAND" } {
		setBrush(QBrush(QImage("villager.fw.png")));
	}

	bool canMove() const override { return true; }

	string getType() const override {
		return this->type;
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


	int getBaseHitChance() const override {
		return this->baseHitChance;
	}
	void modifyBaseHitChance(int newBHC) override {
		this->baseHitChance = newBHC;
	}


	int getCurrentHitChance() const override {
		return this->currentHitChance;
	}
	void modifyCurrentHitChance(int newCHC) override {
		this->currentHitChance = newCHC;
	}
};




class Tower : public AbstractBuilding {
protected:
public:
	Tower(int id, int baseHealth, int damagePerHit, int hitChange) : AbstractBuilding{ id, baseHealth ,damagePerHit, hitChange, "LAND" } {
		setBrush(QBrush(QImage("tower.fw.png")));
	}

	virtual bool canMove() const override { return false; }

	string getType() const override {
		return this->type;
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


	int getBaseHitChance() const override {
		return this->baseHitChance;
	}
	void modifyBaseHitChance(int newBHC) override {
		this->baseHitChance = newBHC;
	}


	int getCurrentHitChance() const override {
		return this->currentHitChance;
	}
	void modifyCurrentHitChance(int newCHC) override {
		this->currentHitChance = newCHC;
	}
};


class Galleon : public AbstractCharacter {
protected:
public:
	Galleon(int id, int baseHealth, int damagePerHit, int hitChange) : AbstractCharacter{ id, baseHealth ,damagePerHit, hitChange, "WATER" } {
		setBrush(QBrush(QImage("galleon.fw.png")));
	}

	virtual bool canMove() const override { return true; }

	string getType() const override {
		return this->type;
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


	int getBaseHitChance() const override {
		return this->baseHitChance;
	}
	void modifyBaseHitChance(int newBHC) override {
		this->baseHitChance = newBHC;
	}


	int getCurrentHitChance() const override {
		return this->currentHitChance;
	}
	void modifyCurrentHitChance(int newCHC) override {
		this->currentHitChance = newCHC;
	}
};
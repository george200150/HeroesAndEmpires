#pragma once
#include <QGraphicsRectItem>
#include <QBrush>
#include <string>
using std::string;



class AbstractUnit : public QGraphicsRectItem {
protected:
	int baseHealth;
	int currentHealth;
	int BaseDamagePerHit;
	int currentDamagePerHit;

	//vector<Action*> actions; - this is gonna be my next headache for this week
public:
	AbstractUnit(int baseHealth, int damagePerHit) : baseHealth{ baseHealth }, BaseDamagePerHit{ damagePerHit }{
		setRect(0, 0, 20, 20);
		currentHealth = baseHealth;
		currentDamagePerHit = damagePerHit;
	}

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
	AbstractCharacter(int baseHealth, int damagePerHit) : AbstractUnit{ baseHealth ,damagePerHit } {
	}

	virtual ~AbstractCharacter() = default;
};


class AbstractBuilding : public AbstractUnit {
protected:
	//int range;
public:
	AbstractBuilding(int baseHealth, int damagePerHit) : AbstractUnit{ baseHealth ,damagePerHit } {
	}

	virtual ~AbstractBuilding() = default;
};


class Villager : public AbstractCharacter {
protected:
	string image;
public:
	Villager(int baseHealth, int damagePerHit) : AbstractCharacter{ baseHealth ,damagePerHit } {
		image = "NULL";
		setBrush(QBrush(QImage("villager.fw.png")));
	}

	string getImage() const {
		return this->image;
	}

	int getBaseHealth() const {
		return this->baseHealth;
	}

	void modifyBaseHealth(int newBH) {
		this->baseHealth = newBH;
	}


	int getCurrentHealth() const {
		return this->currentHealth;
	}

	void modifyCurrentHealth(int newCH) {
		this->currentHealth = newCH;
	}


	int getBaseDamagePerHit() const {
		return this->BaseDamagePerHit;
	}

	void modifyBaseDamagePerHit(int newBDPH) {
		this->BaseDamagePerHit = newBDPH;
	}


	int getCurrentDamagePerHit() const {
		return this->currentDamagePerHit;
	}

	void modifyCurrentDamagePerHit(int newCDPH) {
		this->currentDamagePerHit = newCDPH;
	}
};




class Tower : public AbstractBuilding {
protected:
	string image;
public:
	Tower(int baseHealth, int damagePerHit) : AbstractBuilding{ baseHealth ,damagePerHit } {
		image = "NULL";
	}

	string getImage() const {
		return this->image;
	}

	int getBaseHealth() const {
		return this->baseHealth;
	}

	void modifyBaseHealth(int newBH) {
		this->baseHealth = newBH;
	}


	int getCurrentHealth() const {
		return this->currentHealth;
	}

	void modifyCurrentHealth(int newCH) {
		this->currentHealth = newCH;
	}


	int getBaseDamagePerHit() const {
		return this->BaseDamagePerHit;
	}

	void modifyBaseDamagePerHit(int newBDPH) {
		this->BaseDamagePerHit = newBDPH;
	}


	int getCurrentDamagePerHit() const {
		return this->currentDamagePerHit;
	}

	void modifyCurrentDamagePerHit(int newCDPH) {
		this->currentDamagePerHit = newCDPH;
	}
};
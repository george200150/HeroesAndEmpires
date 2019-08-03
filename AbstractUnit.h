#pragma once
#include <QGraphicsRectItem>
#include <QBrush>
#include <string>
#include <vector>
using std::string;
using std::vector;

/*#include "AbstractAction.h" THIS HAS GONE*/


/*
Class representing a 50 by 50 square on the map.
This is one of the units belonging to a player playing the game.
The unit has health, damage, hit chance visible for the user.
*/
class AbstractUnit : public QGraphicsRectItem {
protected:
	int id;//UNIQUE
	int baseHealth;//health @ spawn moment
	int currentHealth;//health along unit's lifespan
	int BaseDamagePerHit;//damage/hit @ spawn moment
	int currentDamagePerHit;//damage/hit along unit's lifespan
	int baseHitChance;//hit chance @ spawn moment
	int currentHitChance;// hit chance along unit's lifespan
	string type;//LAND or WATER
	string photo;//only the name of the unit (should be the unit's name)
	
	float speed;
	int range;
	//int totalCapacity;
	//int occupiedCapacity;
	//bool canFortify;
	//this implies one more inheritance, as there must be a vector of units on each tile...
	//(composite pattern)
	vector<string> actions;
	vector<string> trainable;
public:

	virtual vector<string> getTrainable() const = 0;

	/*
	Constructor of the AbstractUnit class. This method sets the id, hp, damage, hit chance and the type.
	Also, it automatically sets the shape of the unit to a 50x50 square.
	*/
	AbstractUnit(int id, int baseHealth, int damagePerHit, int hitChance, string type, float speed, int range) :
		id{ id }, baseHealth{ baseHealth }, BaseDamagePerHit{ damagePerHit },
		baseHitChance{ hitChance }, type{ type }, speed{ speed }, range{ range }{
		setRect(0, 0, 50, 50);
		currentHealth = baseHealth;
		currentDamagePerHit = damagePerHit;
		currentHitChance = hitChance;
	}

	/*
	Method that returns an integer representing the speed of the unit.
	The speed determines how many tiles the unit can be moved at the same price as other units.
	i.e. :
	speed = 1   default.
	speed = 2   unit can go twice more than default in one turn.
	speed = 0.5 unit is twice as slower as the default speed.
	*/
	virtual float getSpeed() const = 0;


	

	/*
	Method that returns an integer representing the range of the unit.
	The range determines how far the unit can attack (interact with) other units.
	the value is represented in tiles.
	*/
	virtual int getRange() const = 0;




	/*
	Method that returns as a string the name of the photo of each object.
	The name DOES NOT contain the .extension so that it represents only the base word
	that is going the first string concatenated with the colour of the player.
	This is used when displaying different users' units on the map.
	*/
	virtual string getPhoto() const = 0;



	/*
	Method that sets the name of the photo of each object.
	PRECONDITION: the name MUST NOT include the .extension
	This is used when concatenating the base name with the player colour.
	*/
	virtual void setPhoto(string newPhoto) = 0;


	/*
	
	*/
	virtual vector<string> getActions() = 0;



	/*
	Method that returns a boolean value representing the capability
	of the unit to be moved on the map.
	*/
	virtual bool canMove() const = 0;


	/*
	Method that returns as a string the type of the unit.
	The type can be whether: "LAND" or "WATER", both in caps.
	*/
	virtual string getType() const = 0;



	/*
	Method that returns the unique identifier of the unit.
	*/
	virtual int getId() const = 0;



	/*
	Method that returns the base health of the unit.
	This value CAN ONLY BE MODIFIED by when an upgrade is completed.
	*/
	virtual int getBaseHealth() const = 0;



	/*
	Method that modifies the base health of each unit that is going to spawn from now on.
	This happens ONLY when an upgrade is completed.

	Every unit present on the map will have this base health, and the current health will be
	increased accordingly to the base health gain.
	
	***(this information must be stored by each player so that the ones without the upgrades cannot
	generate more powerful units.)***
	*/
	virtual void modifyBaseHealth(int newBH) = 0;



	/*
	Method that returns an integer representing the current health of the unit
	This value will range from 0 to baseHealth. When it reaches a value <=0 the unit is dead.
	*/
	virtual int getCurrentHealth() const = 0;



	/*
	Method that changes the value of the current health.
	This method is called when the unit is ATTACKED or HEALED.
	*/
	virtual void modifyCurrentHealth(int newCH) = 0;



	/*
	Method that returns an integer representing the damage dealt by the unit / 1 attack action.
	*/
	virtual int getBaseDamagePerHit() const = 0;




	/*
	Method that changes the value of the damage / 1 attack action.
	This value CAN ONLY BE MODIFIED by when an upgrade is completed.
	*/
	virtual void modifyBaseDamagePerHit(int newBDPH) = 0;



	/*
	Method that returns an integer representing the current damage / 1 attack action.
	***(...at the moment it cannot differ from the base value...)***
	*/
	virtual int getCurrentDamagePerHit() const = 0;



	/*
	Method that changes the unit's current damage / 1 attack action.
	*/
	virtual void modifyCurrentDamagePerHit(int newCDPH) = 0;



	/*
	Method that returns an integer representing the hit change of the unit.
	This value can take values from 0 (representing certain miss) to 100 (representing certain hit).
	*/
	virtual int getBaseHitChance() const = 0;



	/*
	Method that changes the hit chance of the unit.
	This value CAN ONLY BE MODIFIED by when an upgrade is completed.
	*/
	virtual void modifyBaseHitChance(int newBHC) = 0;



	/*
	Method that returns an integer representing the current hit chance of the unit.
	*/
	virtual int getCurrentHitChance() const = 0;



	/*
	Method that changes the current hit chance of the unit.
	*/
	virtual void modifyCurrentHitChance(int newCHC) = 0;

	virtual ~AbstractUnit() = default;
};



class AbstractCharacter : public AbstractUnit {
protected:
public:
	AbstractCharacter(int id, int baseHealth, int damagePerHit, int hitChange, string type, float speed, int range) :
		AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type, speed, range } {
	}

	virtual vector<string> getTrainable() const override = 0;
	virtual float getSpeed() const override = 0;
	virtual int getRange() const override = 0;
	virtual string getPhoto() const override = 0;
	virtual void setPhoto(string newPhoto) override = 0;
	virtual vector<string> getActions() override = 0;
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
public:
	AbstractBuilding(int id, int baseHealth, int damagePerHit, int hitChange, string type, float speed, int range) :
		AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type, speed, range } {
	}

	virtual vector<string> getTrainable() const override = 0;
	virtual float getSpeed() const override { return 0; };
	virtual int getRange() const override = 0;
	virtual string getPhoto() const override = 0;
	virtual void setPhoto(string newPhoto) override = 0;
	virtual vector<string> getActions() override = 0;
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
	EmptyUnit(int id, int baseHealth, int damagePerHit, int hitChange, string type, float speed, int range) :
		AbstractUnit{ id, baseHealth ,damagePerHit, hitChange, type, speed, range } {
	}

	virtual vector<string> getTrainable() const override { vector<string> v;  return v; };
	virtual float getSpeed() const override { return -1; }
	virtual int getRange() const override { return -1; };
	virtual string getPhoto() const override { return "-1"; }
	virtual void setPhoto(string newPhoto) override {}
	virtual vector<string> getActions() override { vector<string> v;  return v; }
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
	Villager(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractCharacter{ id, baseHealth ,damagePerHit, hitChange, "LAND", 1, 1 } {
		this->photo = "villager";

		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("ATTACK");
		this->actions.push_back("MOVE");
		//this->actions.push_back("FORTIFY");
		this->actions.push_back("BUILD");
		this->actions.push_back("REPAIR");
	}

	vector<string> getTrainable() const override {
		//return this->trainable;
		vector<string> v;  return v;
	}

	float getSpeed() const override {
		return this->speed;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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
	Tower(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractBuilding{ id, baseHealth ,damagePerHit, hitChange, "LAND", 0, 3 } {
		this->photo = "tower";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("ATTACK");
	}

	//speed == 0;

	vector<string> getTrainable() const override {
		return this->trainable;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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
	Galleon(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractCharacter{ id, baseHealth ,damagePerHit, hitChange, "WATER", 1, 2 } {
		this->photo = "galleon";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("ATTACK");
		this->actions.push_back("MOVE");
		this->actions.push_back("DOCK");
	}

	vector<string> getTrainable() const override {
		//return this->trainable;
		vector<string> v;  return v;
	}

	float getSpeed() const override {
		return this->speed;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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



class HorseArcher : public AbstractCharacter {
protected:
public:
	HorseArcher(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractCharacter{ id, baseHealth ,damagePerHit, hitChange, "LAND", 2, 2 } {
		this->photo = "horse_archer";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("ATTACK");
		this->actions.push_back("MOVE");
	}

	vector<string> getTrainable() const override {
		//return this->trainable;
		vector<string> v;  return v;
	}

	float getSpeed() const override {
		return this->speed;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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


class TownCenter : public AbstractBuilding {
protected:
public:
	TownCenter(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractBuilding{ id, baseHealth ,damagePerHit, hitChange, "LAND", 0, 3 } {
		this->photo = "town_center";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("TRAIN");
		this->trainable.push_back("VILLAGER");
	}

	//speed == 0;

	vector<string> getTrainable() const override {
		return this->trainable;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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

class Dock : public AbstractBuilding {
protected:
public:
	Dock(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractBuilding{ id, baseHealth ,damagePerHit, hitChange, "WATER", 0, 0 } {
		this->photo = "dock";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("TRAIN");
		this->trainable.push_back("GALLEON");
	}

	//speed == 0;

	vector<string> getTrainable() const override {
		return this->trainable;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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

class Barracks : public AbstractBuilding {
protected:
public:
	Barracks(int id, int baseHealth, int damagePerHit, int hitChange) :
		AbstractBuilding{ id, baseHealth ,damagePerHit, hitChange, "LAND", 0, 0 } {
		this->photo = "barracks";
		setBrush(QBrush(QImage(QString::fromStdString(this->photo + ".fw.png"))));
		this->actions.push_back("TRAIN");
		this->trainable.push_back("HORSE ARCHER");
	}

	//speed == 0;

	vector<string> getTrainable() const override {
		return this->trainable;
	}

	int getRange() const override {
		return this->range;
	}

	string getPhoto() const override {
		return this->photo;
	}
	void setPhoto(string newPhoto) override {
		this->photo = newPhoto;
	}

	vector<string> getActions() override {
		return this->actions;
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
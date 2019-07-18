#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;



class AbstractAction{
protected:
	string name;
	int cost;
public:
	AbstractAction(string name, int cost) : name{ name }, cost{ cost } {}

	virtual string getName() const {
		return this->name;
	}

	virtual int getCost() const {
		return this->cost;
	}

	//virtual void execute() = 0;

	virtual ~AbstractAction() = default;
};


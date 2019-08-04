#pragma once
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qboxlayout.h>
#include "GAME.h"

class Menu : public QWidget {
private:
	QPushButton* btnPlay = new QPushButton{"PLAY"};

	QLabel* labP1 = new QLabel{ "PLayer1: " };
	QLabel* labP2 = new QLabel{ "PLayer2: " };

	QLineEdit* txtP1 = new QLineEdit{ "George200150" };
	QLineEdit* txtP2 = new QLineEdit{ "LordOfUltima98" };


	void guiStartup() {
		
		QWidget* wdgName = new QWidget;
		QFormLayout* fl = new QFormLayout;
		wdgName->setLayout(fl);
		fl->addRow(labP1, txtP1);
		fl->addRow(labP2, txtP2);
		
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdgStart = new QWidget;
		QHBoxLayout* lh = new QHBoxLayout;
		wdgStart->setLayout(lh);
		lh->addWidget(btnPlay);
		lh->addWidget(wdgName);

		linit->addWidget(wdgStart);
	}

	void initSignalSlots() {
		QObject::connect(btnPlay, &QPushButton::pressed, this, &Menu::play);
	}

	void initialGuiState() {

	}

public:

	Menu() {
		guiStartup();
		initSignalSlots();
		initialGuiState();
	}

	void play() {
		//this->btnPlay->setEnabled(false);

		GameEngine* engine = new GameEngine{};

		Map* map = new Map{};

		string nameP1 = this->txtP1->text().toStdString();
		string nameP2 = this->txtP2->text().toStdString();
		//should set a limit of characters for each name...

		GAME* view = new GAME{ engine, map, nameP1, nameP2 };


		view->show();
		engine->startGame();
	}
};


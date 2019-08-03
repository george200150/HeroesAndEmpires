#pragma once
#include <qwidget.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include "GAME.h"

class Menu : public QWidget {
private:
	QPushButton* btnPlay = new QPushButton{"PLAY"};

	void guiStartup() {
		QVBoxLayout* linit = new QVBoxLayout;
		this->setLayout(linit);

		QWidget* wdgStart = new QWidget;
		QHBoxLayout* lh = new QHBoxLayout;
		wdgStart->setLayout(lh);
		lh->addWidget(btnPlay);

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

		//while (true) {}

		Map* map = new Map{};

		//while (true) {}

		GAME* view = new GAME{ engine, map };


		//while (true) {}

		view->show();
		engine->startGame();
	}
};


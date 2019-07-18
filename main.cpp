#include <QtWidgets/QApplication>

#include "GAME.h"

int main(int argc, char *argv[]){

	QApplication a(argc, argv);

	GameEngine* engine = new GameEngine{};
	Map* map = new Map{};
	GAME* view = new GAME{ engine, map };
	
	
	view->show();
	engine->startGame();


	return a.exec();
}

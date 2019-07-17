#include <QtWidgets/QApplication>

#include "GAME.h"

int main(int argc, char *argv[]){

	QApplication a(argc, argv);

	GameEngine* engine = new GameEngine{};
	GAME* view = new GAME{ engine };
	
	
	view->show();
	engine->startGame();


	return a.exec();
}

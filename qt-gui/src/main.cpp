#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include "qpoolengine.h"

#include "qpoolrectobject.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Create engine
	std::shared_ptr<QPoolEngine> engine( new QPoolEngine() );
	
	// Create and show GUI
	MainWindow w;
	w.engine(engine);
	engine->connect();
	w.show();
	
	// Create table boundary elements
	engine->createTableBoundary();
	engine->updateScene();
	
	// Run
    return a.exec();

}

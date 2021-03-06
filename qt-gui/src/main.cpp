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
	QPoolEngine* engine = new QPoolEngine();
	
	// Create and show GUI
	MainWindow w;
	w.engine(engine);
	engine->connect();
	w.show();
	
	// Create table boundary elements
	engine->createTableBoundary();
	engine->updateScene();
	
	for (auto qObj : engine->scene()->items())
	{
		qDebug() << "pos of qObj: " << qObj->x() <<", "<< qObj->y();
		qDebug() << "scenePos of qObj: " << qObj->scenePos().x() <<", "<< qObj->scenePos().y();
	}
	// Run
    return a.exec();

}

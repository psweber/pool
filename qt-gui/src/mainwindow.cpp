#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow() :
	ui_(new Ui::MainWindow)
	{
	ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui_;
}


QGraphicsView* MainWindow::view() { return ui_->graphicsView; }
const QGraphicsView* MainWindow::view() const { return ui_->graphicsView; }

void MainWindow::engine(std::shared_ptr<QPoolEngine> engine)
{
	engine_ = engine;
	
	// Set scene for graphics view
	ui_->graphicsView->setScene(engine->scene().get());

	// Tell engine about the main window
	engine->window(this);
}

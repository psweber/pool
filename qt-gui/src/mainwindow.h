#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qpoolengine.h"

namespace Ui {
class MainWindow;
}

class QGraphicsView;

/**
 * @brief The MainWindow class holds the user interface and serves as connector
 * for the engine.
 */
class MainWindow : public QMainWindow
{

private:
	Ui::MainWindow *ui_;

	std::shared_ptr<QPoolEngine> engine_;
    
    
public:
    /**
     * @brief MainWindow Standard constructor
     */
	MainWindow();
	~MainWindow();

    /**
     * @brief engine Set the engine
     * @param engine
     */
	void engine(std::shared_ptr<QPoolEngine> engine);
	const std::shared_ptr<QPoolEngine> engine() const { return engine_; }

    /**
     * @brief view Return the QGraphicsView
     * @return
     */
	QGraphicsView* view();

    /**
     * @brief view Return the QGraphicsView
     * @return
     */
	const QGraphicsView* view() const;

    /**
     * @brief ui Return the user interface
     * @return
     */
	Ui::MainWindow* ui() { return ui_; }

};

#endif // MAINWINDOW_H

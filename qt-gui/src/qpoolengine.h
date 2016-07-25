#ifndef QPOOLENGINE_H
#define QPOOLENGINE_H

#include <QObject>
#include <QList>
#include <QThread>

#include "PoolEngine.hpp"
#include "qpoolobject.h"

class SceneRefresher;
class EngineTicker;
class QGraphicsScene;
class MainWindow;
class QPushButton;

namespace Ui
{
	class MainWindow;
}

/**
 * @brief The QPoolEngine class connects the Qt-GUI with the actual engine
 * The QPoolEngine serves as the communicator to the actual engine. As
 * such, it handles user requests from the GUI like engine control and
 * object creation/manipulation.
 */
class QPoolEngine : public QObject, public engine::PoolEngine
{
    Q_OBJECT

private:
    /**
     * @brief screenSize_ The size of visible screen
     */
	QVector<double> screenSize_;

    /**
     * @brief graphicsScene_ The only graphics scene
     */
    std::shared_ptr<QGraphicsScene> graphicsScene_;
    
    /**
     * @brief ticker_ The ticker responsible for pool engine running
     */
    std::shared_ptr<EngineTicker> ticker_;
    
    /**
     * @brief refresher_ The refresher responsible for graphics scene updating
     */
    std::shared_ptr<SceneRefresher> refresher_;
    
    /**
     * @brief window_ The main window
     */
    MainWindow *window_;
    
    /**
     * @brief ui_ The user interface
     */
    Ui::MainWindow *ui_;
    
    /**
     * @brief tickerThread_ Thread running the ticker
     */
    QThread tickerThread_;

    /**
     * @brief refresherThread_ Thread running the refresher
     */
    QThread refresherThread_;
	
    /**
     * @brief createRectBoundaryObject Creates rectangular boundary object
     * @param centerX Center X position
     * @param centerY Center Y position
     * @param width Width of boundary object
     * @param height Height of boundary object
     */
	void createRectBoundaryObject
	(
		double centerX,
		double centerY,
		double width,
		double height
	);
	
    /**
     * @brief createQPoolObject Creates a QPoolObject according to given shape type
     * @param poolObj The underying pool object
     * @param shType The shape type
     * @return
     */
	QPoolObject* createQPoolObject
	(
		std::shared_ptr<engine::Object> poolObj,
		engine::ShapeType shType
	);
	
    /**
     * @brief readGUIShapeType Return shape type as given in GUI drop box
     * @return Specificd shape type
     */
	engine::ShapeType readGUIShapeType() const;

public:

    /**
     * @brief QPoolEngine Standard constructor
     *
     * Initializes screen, scene, ticker, and refresher
     */
    QPoolEngine();
	~QPoolEngine()
	{
        tickerThread_.quit();
        tickerThread_.wait();

        refresherThread_.quit();
        refresherThread_.wait();
	}

    /**
     * @brief screenSizeX Returns screen width
     * @return
     */
	double screenSizeX() const { return screenSize_[0]; }

    /**
     * @brief screenSizeY Returns screen height
     * @return
     */
	double screenSizeY() const { return screenSize_[1]; }
	
    /**
     * @brief scene Returns graphics scene
     * @return
     */
	std::shared_ptr<QGraphicsScene> scene() { return graphicsScene_; }

    /**
     * @brief scene Returns graphics scene
     * @return
     */
	const std::shared_ptr<QGraphicsScene> scene() const { return graphicsScene_; }
	
    /**
     * @brief ticker Returns ticker
     * @return
     */
	std::shared_ptr<EngineTicker> ticker() { return ticker_; }

    /**
     * @brief ticker Returns ticker
     * @return
     */
	const std::shared_ptr<EngineTicker> ticker() const { return ticker_; }
	
    /**
     * @brief window Sets main window
     * @param window
     */
	void window(MainWindow* window);

    /**
     * @brief window Returns main window
     * @return
     */
	MainWindow* window() { return window_; }

    /**
     * @brief window Returns main window
     * @return
     */
	const MainWindow* window() const { return window_; }
	
    /**
     * @brief ui Returns user interface
     * @return
     */
	Ui::MainWindow* ui() { return ui_; }

    /**
     * @brief ui Returns user interface
     * @return
     */
	const Ui::MainWindow* ui() const { return ui_; }

    /**
     * @brief connect Connect GUI signals to slots
     */
	void connect();

    /**
     * @brief createTableBoundary Create boundary elements for table
     */
    void createTableBoundary();
    
    /**
     * @brief updateObjects Update objects from pool engine
     */
	void updateObjects();
	
    /**
     * @brief updateScene Update object representation on screen
     */
	void updateScene();
    
    /**
     * @brief updateObjectVelocity Update velocity of moving object
     * @param movObj
     */
    void updateObjectVelocity(std::shared_ptr<engine::MovingObject> movObj);

    /**
     * @brief ticker Returns ticker Update rectangular shape with data from GUI
     * @return
     */
    void updateRectShapeFromGUI(std::shared_ptr<engine::Object> poolObj);
    
    /**
     * @brief updateEllipseShapeFromGUI Update ellipsoidal shape with data from GUI
     * @param poolObj
     */
    void updateEllipseShapeFromGUI(std::shared_ptr<engine::Object> poolObj);
    
    /**
     * @brief createMovingObject Creates a moving object with given shape at center
     * @param center
     * @param shType
     * @return
     */
    QPoolObject* createMovingObject(QPoint center,engine::ShapeType shType);

    /**
     * @brief updateObjectFromGUI Update object with data from GUI
     * @param qObj
     * @param guiShType
     */
    void updateObjectFromGUI(QPoolObject* qObj,engine::ShapeType guiShType);
    
    /**
     * @brief printObjects Print object information to qDebug
     */
	void printObjects() const;
	
    /**
     * @brief objectById Return object with given id from graphics scene
     * @param id
     * @return
     */
	QPoolObject* objectById(int id) const;
	
    /**
     * @brief objectByGuiID Return object with id read from GUI
     * @param allowBoundary If true, boundary objects can be picked
     * @return
     */
	QPoolObject* objectByGuiID(bool allowBoundary = false) const;

    /**
     * @brief deleteObject Delete given object
     * @param qObj
     */
	void deleteObject(QPoolObject* qObj);
	
    /**
     * @brief updateButtonBg Update background color of given push button
     * @param button
     * @param c
     */
	void updateButtonBg(QPushButton* button, const QColor& c);
	
    /**
     * @brief printObjectTypes Print object type information to qDebug
     */
	void printObjectTypes();
	
public slots:
	void slotCreateMovingObject();
	void slotDeleteObject();
    void slotUpdateObject();
	void slotUpdateScene();
	void slotStopTicker();
	void slotStopRefresher();
	void slotEnableStartButton();
	void slotDisableStartButton();
	void slotEnablePauseButton();
	void slotDisablePauseButton();
	void slotChooseFillColor();
	void slotChooseBorderColor();
	void slotRandomVelocity();
};

#endif // QPOOLENGINE_H

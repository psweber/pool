
#include <valarray>
#include <QGraphicsScene>
#include "qpoolengine.h"
#include "qpoolrectobject.h"
#include "qpoolellipseobject.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>

# include <random>
# include <unistd.h>

#include "scenerefresher.h"
#include "engineticker.h"


typedef std::valarray<double> ValArray;

QPoolEngine::QPoolEngine() :
	screenSize_({550,481}),
    graphicsScene_(new QGraphicsScene(this)),
    ticker_(new EngineTicker(this)),
    refresher_(new SceneRefresher(this))
{
	ticksPerSecond(60);
    ticker_->moveToThread(&tickerThread_);
    tickerThread_.start();
    
    refresher_->framesPerSecond(60);
    refresher_->moveToThread(&refresherThread_);
    //refresherThread_.start();
    
    table().createCollisionSet(engine::ObjectType::BOUNDARY);
    
    //table().createCollisionSet(engine::ObjectType::MOVING);
    
    
    table().createCollisionSet
    (
		engine::ShapeType::RECTANGLE,
		engine::ShapeType::RECTANGLE,
		engine::ObjectType::MOVING
	);
	
}

void QPoolEngine::connect()
{
	// Connect create button with slotCreateMovingObject
	QObject::connect(ui()->createButton, SIGNAL(clicked()), this, SLOT(slotCreateMovingObject()));
	QObject::connect(ui()->createButton, SIGNAL(clicked()), this, SLOT(slotRandomVelocity()));
	
	// Connect update button with slotUpdateMovingObject
	QObject::connect(ui()->updateButton, SIGNAL(clicked()), this, SLOT(slotUpdateObject()));

	// Connect delete button with slotDeleteMovingObject
	QObject::connect(ui()->deleteButton, SIGNAL(clicked()), this, SLOT(slotDeleteObject()));

	// Connect start button to ticker and refresher start
	QObject::connect(ui()->startButton, SIGNAL(clicked()), ticker_, SLOT(start()));
	QObject::connect(ui()->startButton, SIGNAL(clicked()), refresher_, SLOT(start()));
	
	// Connect pause button to ticker and refresher stop
	QObject::connect(ui()->pauseButton, SIGNAL(clicked()), this, SLOT(slotStopTicker()));
	QObject::connect(ui()->pauseButton, SIGNAL(clicked()), this, SLOT(slotStopRefresher()));

	// Connect start button to enable pause and disable start
	QObject::connect(ui()->startButton, SIGNAL(clicked()), this, SLOT(slotDisableStartButton()));
	QObject::connect(ui()->startButton, SIGNAL(clicked()), this, SLOT(slotEnablePauseButton()));
	
	// Connect pause button to enable start and disable pause
	QObject::connect(ui()->pauseButton, SIGNAL(clicked()), this, SLOT(slotDisablePauseButton()));
	QObject::connect(ui()->pauseButton, SIGNAL(clicked()), this, SLOT(slotEnableStartButton()));
	
	// Connect fill color button
    updateButtonBg(ui()->fillColorButton, QPoolObject::fillColor());
	QObject::connect(ui()->fillColorButton, SIGNAL(clicked()), this, SLOT(slotChooseFillColor()));
	
	// Connect border color button
    updateButtonBg(ui()->borderColorButton, QPoolObject::borderColor());
	QObject::connect(ui()->borderColorButton, SIGNAL(clicked()), this, SLOT(slotChooseBorderColor()));

	// Connect random button
	QObject::connect(ui()->randomButton, SIGNAL(clicked()), this, SLOT(slotRandomVelocity()));

	/*
	QObject::connect
	(
		scene(),
		SIGNAL(changed(const QList<QRectF> &) ),
		ui()->graphicsView,
		SLOT(updateScene( const QList<QRectF> &))
	);
	*/
}

void QPoolEngine::window(MainWindow* window)
{
	window_=window;
	ui_=window->ui();
}

/**
 * @brief QPoolEngine::createRectTableBoundary Uses rectangular boundary objects to form a rectangular pool table
 * @param sizeX Width of table
 * @param sizeY Height of table
 */
void QPoolEngine::createTableBoundary()
{
	double width = screenSizeX();
	double height = screenSizeY();
	
	double relBoundaryThickness = 0.004;
	
	// Let the PoolEngine create the boundary
	table().createBoundary(width, height, relBoundaryThickness);
	
	// Take the boundary elements and add them to objectList
	for (auto statObj : table().boundaryObjects())
	{
		// Based on the engine object, create QPoolObject
		QPoolRectObject* qObj
		(
			new QPoolRectObject(this,statObj)
		);
		
		qObj->setBrush(QColor(50,50,50));
		scene()->addItem(qObj);
		qObj->setParent(scene());
	}
}

QPoolObject* QPoolEngine::createMovingObject
(
	QPoint center,
	engine::ShapeType shType
)
{
	// Create moving object for pool engine
	std::valarray<double> c({(double) center.x(), (double) center.y()});
	
	std::shared_ptr<engine::MovingObject> engineObject
	(
		table().createObject<engine::MovingObject>
		(
			table().movingObjects(),
			c,
			shType
		)
	);
	
	engineObject->shape()->center(c);
	
	// Create wrapping QPoolObject
	QPoolObject* qObj
	(
		createQPoolObject
		(
			engineObject,
			shType
		)
	);
	
	// Add to scene
	switch (shType)
	{
		case engine::ShapeType::RECTANGLE:
			scene()->addItem(qObj->asRect());
			graphicItems_.append(qObj->asRect());
			break;
		
		case engine::ShapeType::ELLIPSE:
			scene()->addItem(qObj->asEllipse());
			graphicItems_.append(qObj->asEllipse());
			break;
		
		default:
			break;
	}
	
	qObj->setParent(scene());
	return qObj;
}

engine::ShapeType QPoolEngine::readGUIShapeType() const
{
	QString shapeBoxStr(ui()->shapeBox->currentText());
	
	if ( shapeBoxStr == QString("Rectangle"))
	{
			return engine::ShapeType::RECTANGLE;
	} 
	else if ( shapeBoxStr == QString("Ellipse"))
	{
			return engine::ShapeType::ELLIPSE;
	}
	else
	{
		qDebug() << "ERROR: Incorrect string in shape box in "
			<< "QPoolEngine::slotCreateMovingObject()";
		return engine::ShapeType::NONE;
	}
}

void QPoolEngine::slotCreateMovingObject()
{
	engine::ShapeType guiShType( readGUIShapeType() );
	
	// Create QPoolObject, positioned at screen center
	QPoolObject* qObj
	(
		createMovingObject
		(
			QPoint(0.5*screenSizeX(),0.5*screenSizeY()),
			guiShType
		)
	);
	
	// Read and apply properties from GUI
	updateObjectFromGUI(qObj,guiShType);
}

void QPoolEngine::updateObjectFromGUI
(
	QPoolObject* qObj,
	engine::ShapeType guiShType
)
{
	// Change shape type
	if (guiShType != qObj->poolObject()->shape()->type())
	{
		QPoolObject::changeShapeType(this,qObj,guiShType);
	}
	
	// Update velocity if moving object
	if (qObj->poolObject()->type() == engine::ObjectType::MOVING)
	{
		// Get the underlying moving pool object
		std::shared_ptr<engine::MovingObject> movObj
		(
			std::static_pointer_cast<engine::MovingObject>
			(
				qObj->poolObject()
			)
		);
		
		// Update velocity
		updateObjectVelocity(movObj);
	}
	

	// Update object shape properties
	switch (guiShType)
	{
		case engine::ShapeType::RECTANGLE:
			updateRectShapeFromGUI(qObj->poolObject());
			break;
			
		case engine::ShapeType::ELLIPSE:
			updateEllipseShapeFromGUI(qObj->poolObject());
			break;
		
		default:
			break;
	}
	
	
	// Update appearance
	qObj->setBrush(QPoolObject::fillColor());
	qObj->setPen(QPen(QPoolObject::borderColor(), 1));
	
	
	// Update graphical representation
	qObj->updateFromEngine();
}

QPoolObject* QPoolEngine::objectByGuiID(bool allowBoundary) const
{
	// Get object id
	int id ( ui()->idBox->text().toInt() );
	
	// Get QPoolObject
	QPoolObject* qObj( objectById(id) );
	
	// Abort if object not found
	if (!qObj)
	{
		QMessageBox msgBox;
		QString text( QString("Invalid object ID (%1)").arg(id) );
		msgBox.setText(text);
		msgBox.exec();
		return 0;
	}
	
	// Abort if not-allowed boundary picked
	if
	(
		!allowBoundary
	 && (qObj->poolObject()->type() == engine::ObjectType::BOUNDARY)
	)
	{
		QMessageBox msgBox;
		msgBox.setText("Cannot modify boundary objects!");
		msgBox.exec();
		return 0;
	}
	
	// Everything ok, return object
	return qObj;
}

void QPoolEngine::deleteObject(QPoolObject* qObj)
{
	// Delete from scene
	switch (qObj->poolObject()->shape()->type())
	{
		case engine::ShapeType::RECTANGLE:
			scene()->removeItem(qObj->asRect());
			break;
		
		case engine::ShapeType::ELLIPSE:
			scene()->removeItem(qObj->asEllipse());
			break;
		
		default:
			break;
	}
	
	// Delete QPoolObject
	delete qObj;
}

void QPoolEngine::slotDeleteObject()
{
	// Get object
	QPoolObject* qObj( objectByGuiID() );
	
	// Delete
	if (qObj)
	{
		deleteObject(qObj);
	}
}

void QPoolEngine::slotUpdateObject()
{
	// Get shape type
	engine::ShapeType shType( readGUIShapeType() );
	
	// Get object
	QPoolObject* qObj( objectByGuiID() );
	
	// Update
	if (qObj)
	{
		updateObjectFromGUI(qObj,shType);
	}
}

void QPoolEngine::updateObjectVelocity(std::shared_ptr<engine::MovingObject> movObj)
{
	double angle( ui()->angleSpinBox->value() );
	double speed( ui()->speedSpinBox->value() );
	double vx ( std::sin(angle/360.0 * 2.0 * 3.141592) * speed );
	double vy ( std::cos(angle/360.0 * 2.0 * 3.141592) * speed );
	
	movObj->velocity(ValArray({vx,vy}));
}

// Returns object with given id
QPoolObject* QPoolEngine::objectById(int id) const
{
	for (auto item : scene()->items())
	{
		QPoolObject *qObj(dynamic_cast<QPoolObject*>(item));
		
		if (qObj->poolObject()->id() == id)
		{
			return qObj;
		}
	}
	
	return 0;
}

void QPoolEngine::updateRectShapeFromGUI(std::shared_ptr<engine::Object> poolObj)
{

	// Get data from GUI
	int aspectRatio( ui()->aspectRatioBox->value() );
	int size( ui()->sizeBox->value() );
	Qt::CheckState horizontal( ui()->horizontalBox->checkState() );
	
	double width = size, height = size;
	
	if (horizontal == Qt::Checked)
	{
		width *= aspectRatio;
	}
	else
	{
		height *= aspectRatio;
	}
	
	// Set data
	std::shared_ptr<engine::RectangleShape> rectShape
	(
		std::static_pointer_cast<engine::RectangleShape>
		(
			poolObj->shape()
		)
	);
	
	rectShape->size(width,height);
}

void QPoolEngine::updateEllipseShapeFromGUI(std::shared_ptr<engine::Object> poolObj)
{
	// Get data from GUI
	int aspectRatio( ui()->aspectRatioBox->value() );
	int size( ui()->sizeBox->value() );
	Qt::CheckState horizontal( ui()->horizontalBox->checkState() );
	
	double axisX = 0.5*size, axisY = 0.5*size;
	
	if (horizontal == Qt::Checked)
	{
		axisX *= aspectRatio;
	}
	else
	{
		axisY *= aspectRatio;
	}
	
	// Set data
	std::shared_ptr<engine::EllipseShape> ellipseShape
	(
		std::static_pointer_cast<engine::EllipseShape>
		(
			poolObj->shape()
		)
	);
	
	ellipseShape->size(axisX,axisY);
}

QPoolObject* QPoolEngine::createQPoolObject
(
	std::shared_ptr<engine::Object> engObj,
	engine::ShapeType shType
)
{
	// Based on the engine object, create QPoolObject
	QPoolObject* qObj;
	
	switch (shType)
	{
		case engine::ShapeType::RECTANGLE:
			qObj = new QPoolRectObject(this,engObj);
			return qObj;
			
		case engine::ShapeType::ELLIPSE:
			qObj = new QPoolEllipseObject(this,engObj);
			return qObj;
		
		default:
			qDebug() << "ERROR: Incorrect ShapeType in "
				<< "QPoolEngine::createQPoolObject()";
			return 0;
	}
	
	return 0;
}

void QPoolEngine::printObjects() const
{
	qDebug() << "QPoolEngine::printObjectBounds() (taken from scene)";
    for (auto& obj : scene()->items()) //objectList_)
    {
		qreal x, y, width, height;
		obj->boundingRect().getRect(&x,&y,&width,&height);
		x = obj->scenePos().x();
		y = obj->scenePos().y();
		
		qDebug() << "x=" << x << ", y=" << y << ", width=" << width << ", height=" << height;
	}
}

void QPoolEngine::updateObjects()
{
	qDebug() << "items in scene: " << scene()->items().size();
	for (auto item : graphicItems_)
	{
		QPoolObject *qObj(dynamic_cast<QPoolObject*>(item));
		
		qObj->updateFromEngine();
	}
}

void QPoolEngine::printObjectTypes()
{
	
	int i=0;
	for (auto item : scene()->items())
	{
		QPoolObject *qObj(dynamic_cast<QPoolObject*>(item));
		
		switch (qObj->poolObject()->shape()->type())
		{
			case engine::ShapeType::ELLIPSE:
				qDebug() << "item " << i++ << " is  ELLIPSE";
				break;
				
			case engine::ShapeType::RECTANGLE:
				qDebug() << "item " << i++ << " is  RECT";
				break;
			
			default:
				break;
		}
		
	}
}

void QPoolEngine::updateScene()
{
	scene()->update
	(
		0,
		0,
		screenSizeX(),
		screenSizeY()
	);
}

void QPoolEngine::slotUpdateScene()
{
	tick();
	updateObjects();
}

void QPoolEngine::slotStopTicker()
{
	ticker_->stop(); //_ = true;
}

void QPoolEngine::slotStopRefresher()
{
	refresher_->stop();
}

void QPoolEngine::slotEnableStartButton()
{
	ui()->startButton->setDisabled(false);
}

void QPoolEngine::slotEnablePauseButton()
{
	ui()->pauseButton->setDisabled(false);
}

void QPoolEngine::slotDisableStartButton()
{
	ui()->startButton->setDisabled(true);
}

void QPoolEngine::slotDisablePauseButton()
{
	ui()->pauseButton->setDisabled(true);
}

void QPoolEngine::slotChooseFillColor()
{
	QColor fillColor( QColorDialog::getColor() );
	updateButtonBg(ui()->fillColorButton, fillColor);
	QPoolObject::fillColor(fillColor);
}

void QPoolEngine::slotChooseBorderColor()
{
	QColor borderColor( QColorDialog::getColor() );
	updateButtonBg(ui()->borderColorButton, borderColor);
	QPoolObject::borderColor(borderColor);
}

void QPoolEngine::updateButtonBg(QPushButton* button, const QColor& c)
{
	QString fillStr =
		QString("background-color: rgb(%1,%2,%3);")
			.arg( c.red() ).arg( c.green() ).arg( c.blue() );
	
	button->setStyleSheet( fillStr );
}


void QPoolEngine::slotRandomVelocity()
{
	// Setup random generator
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> angleDist(0, 359);
	std::uniform_real_distribution<double> speedDist(15, 300);
	
	// Generate angle and speed
	int angle = (int) angleDist(mt);
	int speed = (int) speedDist(mt);
	
	// Apply to GUI
	ui()->angleSpinBox->setValue(angle);
	ui()->speedSpinBox->setValue(speed);
}

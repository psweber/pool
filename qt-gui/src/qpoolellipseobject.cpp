#include "qpoolellipseobject.h"
#include "qpoolengine.h"
#include "ui_mainwindow.h"

#include <QDebug>

QPoolEllipseObject::QPoolEllipseObject
(
	QPoolEngine* engine,
	std::shared_ptr<engine::Object> poolObject
)
:
QPoolObject(engine,poolObject)
{
	// Get geometric properties
    std::shared_ptr<engine::EllipseShape> ellipseShape
    (
        std::static_pointer_cast<engine::EllipseShape>(poolObject->shape())
    );
	
	double centerX = poolObject->shape()->center()[0];
	double centerY = poolObject->shape()->center()[1];
    double axisX = ellipseShape->axisX();
	double axisY = ellipseShape->axisY();
	
	// Update 
	this->setRect
	(
		centerX-axisX,
		centerY-axisY,
		2.0*axisX,
		2.0*axisY
    );
}

void QPoolEllipseObject::size(double axisX, double axisY)
{
	// Set size of engineObject
    std::shared_ptr<engine::EllipseShape> ellipseShape
    (
        std::static_pointer_cast<engine::EllipseShape>(poolObject()->shape())
    );
	ellipseShape->size(axisX,axisY);
	
	QRectF newRect(this->rect());
	newRect.setWidth(2.0*axisX);
	newRect.setHeight(2.0*axisY);
	
	this->setRect(newRect);
}

void QPoolEllipseObject::updateFromEngine()
{
	// Get ellipse engineObject
    std::shared_ptr<engine::EllipseShape> ellipseShape
    (
        std::static_pointer_cast<engine::EllipseShape>(poolObject()->shape())
    );
	
	// Create new rect
	double centerX = ellipseShape->center()[0];
	double centerY = ellipseShape->center()[1];
    double axisX = ellipseShape->axisX();
	double axisY = ellipseShape->axisY();
	
	QRectF qRect
	(
		centerX-axisX,
		centerY-axisY,
		2.0*axisX,
		2.0*axisY
	);
	
	// Update ellipse
	this->setRect(qRect);
}

void QPoolEllipseObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Update shape-indepentent properties in GUI
	QPoolObject::mousePressEvent(event);
	
	
	// Update geometry in GUI
	std::shared_ptr<engine::EllipseShape> rectShape
	(
		std::static_pointer_cast<engine::EllipseShape>
		(
			poolObject()->shape()
		)
	);
	
	double axisX ( rectShape->axisX() );
	double axisY ( rectShape->axisY() );
	
	int size ( (axisX<axisY) ? axisX : axisY );
	bool horizontal ( (axisX<axisY) ? false : true );
	int ratio ( horizontal ? axisX/axisY : axisY/axisX );
	
	engine()->ui()->sizeBox->setValue( size );
	engine()->ui()->aspectRatioBox->setValue ( ratio );
	engine()->ui()->horizontalBox->setChecked( horizontal );
}

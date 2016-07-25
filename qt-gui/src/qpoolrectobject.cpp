#include "qpoolrectobject.h"
#include "qpoolengine.h"
#include "ui_mainwindow.h"

#include <QDebug>

QPoolRectObject::QPoolRectObject
(
	QPoolEngine* engine,
	std::shared_ptr<engine::Object> poolObject
)
:
QPoolObject(engine,poolObject)
{
	// Compute geometric properties
    std::shared_ptr<engine::RectangleShape> rectShape
    (
        std::static_pointer_cast<engine::RectangleShape>(poolObject->shape())
    );
	
	double centerX = poolObject->shape()->center()[0];
	double centerY = poolObject->shape()->center()[1];
    double w = rectShape->width();
	double h = rectShape->height();

	this->setRect
	(
		centerX-0.5*w,
		centerY-0.5*h,
		rectShape->width(),
		rectShape->height()
    );
}

void QPoolRectObject::size(double width, double height)
{
	// Set size of engineObject
    std::shared_ptr<engine::RectangleShape> rectShape
    (
        std::static_pointer_cast<engine::RectangleShape>(poolObject()->shape())
    );
	rectShape->size(width,height);
	
	QRectF newRect(this->rect());
	newRect.setWidth(width);
	newRect.setHeight(height);
	
	this->setRect(newRect);
}

void QPoolRectObject::updateFromEngine()
{
	// Get rectangular engineObject
    std::shared_ptr<engine::RectangleShape> rectShape
    (
        std::static_pointer_cast<engine::RectangleShape>(poolObject()->shape())
    );
    
	// Create new rect
	double centerX = rectShape->center()[0];
	double centerY = rectShape->center()[1];
    double w = rectShape->width();
	double h = rectShape->height();
	
	QRectF qRect
	(
		centerX-0.5*w,
		centerY-0.5*h,
		rectShape->width(),
		rectShape->height()
	);
	
	// Update rectangle
	this->setRect(qRect);
}

void QPoolRectObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPoolObject::mousePressEvent(event);
	
	std::shared_ptr<engine::RectangleShape> rectShape
	(
		std::static_pointer_cast<engine::RectangleShape>
		(
			poolObject()->shape()
		)
	);
	
	// Update geometry
	double width ( rectShape->width() );
	double height ( rectShape->height() );
	
	int size ( (width<height) ? width : height );
	bool horizontal ( (width<height) ? false : true );
	int ratio ( horizontal ? width/height : height/width );
	
	engine()->ui()->sizeBox->setValue( size );
	engine()->ui()->aspectRatioBox->setValue ( ratio );
	engine()->ui()->horizontalBox->setChecked( horizontal );
}

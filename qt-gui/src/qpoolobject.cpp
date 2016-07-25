#include "qpoolobject.h"
#include "qpoolrectobject.h"
#include "qpoolellipseobject.h"
#include "qpoolengine.h"

#include "ui_mainwindow.h"

#include <QDebug>

QColor QPoolObject::fillColor_ = QColor(100,100,255);
QColor QPoolObject::borderColor_ = QColor(0,0,0);

QPoolObject::QPoolObject
(
	QPoolEngine* engine,
	std::shared_ptr<engine::Object> poolObject
)
:
engine_(engine),
poolObj_(poolObject)
{}

QPoolObject::~QPoolObject()
{
	// Delete from engine
	engine()->table().deleteObject(poolObject());
}

QVector<double> QPoolObject::velocity()
{
	// If object is moving
	if (poolObject()->isMoving())
	{
		std::shared_ptr<engine::MovingObject> engMovObj
		(
			std::static_pointer_cast<engine::MovingObject>
			(
				poolObject()
			)
		);
		
		QVector<double> vel(2);
		vel[0] = engMovObj->velocity()[0];
		vel[2] = engMovObj->velocity()[1];
		return vel;
	}
	
	// If object is static
	else
	{
		return QVector<double>(2,0);
	}
}


QPoolObject* QPoolObject::asQPoolObject(QGraphicsItem* gItem)
{
	QPoolRectObject* rectObj = dynamic_cast<QPoolRectObject*>(gItem);
	
	if (rectObj)
	{
		return dynamic_cast<QPoolObject*>(rectObj);
	}
	
	QPoolEllipseObject* ellipseObj = dynamic_cast<QPoolEllipseObject*>(gItem);
	
	if (ellipseObj)
	{
		return dynamic_cast<QPoolObject*>(ellipseObj);
	}
	
	return 0;
}

void QPoolObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Do nothing in case of boundary object
	if (poolObject()->type() == engine::ObjectType::BOUNDARY)
	{
		return;
	}
	
	// Update ID box
	QString idStr(QString::number(poolObject()->id()));
	engine()->ui()->idBox->setText(idStr);
	
	// Update shape
	switch (poolObject()->shape()->type())
	{
		case engine::ShapeType::RECTANGLE:
			engine()->ui()->shapeBox->setCurrentText("Rectangle");
			break;
			
		case engine::ShapeType::ELLIPSE:
			engine()->ui()->shapeBox->setCurrentText("Ellipse");
			break;
		
		default:
			break;
	}
	
	
	// Update velocity
	std::shared_ptr<engine::MovingObject> movObj
	(
		std::static_pointer_cast<engine::MovingObject>
		(
			poolObject()
		)
	);
	
	double vx ( movObj->vx() );
	double vy ( movObj->vy() );
	
	int speed ( sqrt( vx*vx + vy*vy ) );
	double angle;
	
	double pi = 3.141592;
	double fac = 360.0/(2.0*pi);
	
	if (vy < 0)
	{
		angle = ( atan( vx/vy ) + pi ) * fac;
	}
	else if (vx < 0)
	{
		angle = ( atan( vx/vy ) + 2.0*pi ) * fac;
	}
	else
	{
		angle = ( atan( vx/vy ) ) * fac;
	}
	
	engine()->ui()->angleSpinBox->setValue((int)angle);
	engine()->ui()->speedSpinBox->setValue(speed);
	
	// Update appearance
	// Fill color
	engine()->updateButtonBg
	(
		engine()->ui()->fillColorButton,
		this->brush().color()
	);
	
	// Border color
	engine()->updateButtonBg
	(
		engine()->ui()->borderColorButton,
		this->pen().color()
	);
}

void QPoolObject::changeShapeType
(
	QPoolEngine *engine,
	QPoolObject *obj, 
	engine::ShapeType toShType
)
{
	// Create new QPoolObject with new shape type
	QPoolObject *newObj = 0;
	
	switch(toShType)
	{
		case engine::ShapeType::RECTANGLE:
			newObj = new QPoolRectObject(obj->engine(), obj->poolObject());
			break;
			
		case engine::ShapeType::ELLIPSE:
			newObj = new QPoolEllipseObject(obj->engine(), obj->poolObject());
			break;
		
		default:
			break;
	}
	
	newObj->poolObject()->changeShape(toShType);
	
	// Copy center
	newObj->poolObject()->shape()->center( obj->poolObject()->shape()->center() );

	// Replace graphicsItem in scene
	QGraphicsItem *gObj = dynamic_cast<QGraphicsItem*>(obj);
	engine->scene()->removeItem(gObj);
	engine->scene()->addItem(dynamic_cast<QGraphicsItem*>(newObj));

	
	obj = newObj;
}

#ifndef QPOOLELLIPSEOBJECT_H
#define QPOOLELLIPSEOBJECT_H

#include <QGraphicsEllipseItem>
#include "qpoolobject.h"

/**
 * @brief The QPoolEllipseObject class is the graphical representation of
 * the ellipse object used by the pool engine.
 */
class QPoolEllipseObject : public QPoolObject, public QGraphicsEllipseItem
{
protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		
public:
    QPoolEllipseObject
    (
		QPoolEngine* engine,
		std::shared_ptr<engine::Object> poolObject
	);
	
    /**
     * @brief size Set size of ellipse
     * @param axisX Size of X semi-axis
     * @param axisY Size of Y semi-axis
     */
	void size(double axisX, double axisY);
    
    /**
     * @brief updateFromEngine Read current data from pool engine
     */
    virtual void updateFromEngine();
    
    virtual QBrush brush() const { return QGraphicsEllipseItem::brush(); }
	virtual QPen pen() const { return QGraphicsEllipseItem::pen(); }
    virtual void setBrush(const QBrush &brush) { QGraphicsEllipseItem::setBrush(brush); };
	virtual void setPen(const QPen &pen) {  QGraphicsEllipseItem::setPen(pen); };
	
    /**
     * @brief asEllipse Polymorphic wrapper
     * @return
     */
	virtual QGraphicsEllipseItem* asEllipse() { return this; }
};

#endif // QPOOLELLIPSEOBJECT_H

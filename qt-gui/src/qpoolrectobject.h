#ifndef QPOOLRECTOBJECT_H
#define QPOOLRECTOBJECT_H

#include <QGraphicsRectItem>
#include "qpoolobject.h"


/**
 * @brief The QPoolRectObject class is the graphical representation of
 * the rectangle object used by the pool engine.
 */
class QPoolRectObject : public QPoolObject, public QGraphicsRectItem
{
protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		
public:
    QPoolRectObject
    (
		QPoolEngine* engine,
		std::shared_ptr<engine::Object> poolObject
	);
	
    /**
     * @brief size Update geometry according to given width and height
     * @param width
     * @param height
     */
	void size(double width, double height);
    
    /**
     * @brief updateFromEngine Read current data from pool engine
     */
    virtual void updateFromEngine();
    
    virtual QBrush brush() const { return QGraphicsRectItem::brush(); }
	virtual QPen pen() const { return QGraphicsRectItem::pen(); }
    virtual void setBrush(const QBrush &brush) { QGraphicsRectItem::setBrush(brush); };
	virtual void setPen(const QPen &pen) {  QGraphicsRectItem::setPen(pen); };
	
    /**
     * @brief asRect Polymorphic wrapper
     * @return
     */
	virtual QGraphicsRectItem* asRect() { return this; }
};

#endif // QPOOLRECTOBJECT_H

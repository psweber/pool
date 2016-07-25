#ifndef QPOOLOBJECT_H
#define QPOOLOBJECT_H

#include <QtGui>
#include "PoolEngine.hpp"

class QGraphicsSceneMouseEvent;
class QPoolEngine;
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

/**
 * @brief The QPoolObject class is the abstract base class for all graphical
 * representations of pool objects. It provides polymorphic access to the
 * object updating.
 */
class QPoolObject : public QObject
{
private:
		QPoolEngine* engine_;
		
        std::shared_ptr<engine::Object> poolObj_;
        
        static QColor fillColor_;
        static QColor borderColor_;

protected:
        /**
         * @brief mousePressEvent Handles mouse clicks on objects in screen
         * @param event
         */
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
    QPoolObject
    (
		QPoolEngine* engine,
		std::shared_ptr<engine::Object> poolObject
	);
	
	virtual ~QPoolObject();
	
    /**
     * @brief fillColor Returns object fill color
     * @return
     */
	static QColor fillColor() { return fillColor_; }

    /**
     * @brief fillColor Sets object fill color
     * @param c
     */
	static void fillColor(const QColor& c) { fillColor_ = c; }
	
    /**
     * @brief borderColor Returns object border color
     * @return
     */
	static QColor borderColor() { return borderColor_; }

    /**
     * @brief borderColor Sets object border color
     * @param c
     */
	static void borderColor(const QColor& c) { borderColor_ = c; }
	
    /**
     * @brief changeShapeType Changes shape type of underlying pool object
     * @param engine The QPoolEngine for correcting the scene
     * @param obj The QPoolObject the shape of which shall be changed
     * @param toShType The shape to change to
     */
	static void changeShapeType
	(
		QPoolEngine *engine,
		QPoolObject *obj,
		engine::ShapeType toShType
	);
	
    
    QPoolEngine* engine() { return engine_; }
    const QPoolEngine* engine() const { return engine_; }
    
    /**
     * @brief poolObject Return underlying pool object
     * @return
     */
    std::shared_ptr<engine::Object> poolObject() { return poolObj_; }
    
    QVector<double> velocity();
    const QVector<double>& velocity() const;
    
    /**
     * @brief updateFromEngine Request object update with data from pool engine
     */
    virtual void updateFromEngine() = 0;
    
    virtual QBrush brush() const { return QBrush(); }
	virtual QPen pen() const { return QPen(); }
    virtual void setBrush(const QBrush &brush) {}
	virtual void setPen(const QPen &pen) {}
	
    /**
     * @brief asEllipse If object is ellipse, return as such, else null
     * @return
     */
	virtual QGraphicsEllipseItem* asEllipse() { return 0; }
		
    /**
     * @brief asRect If object is rectangle, return as such, else null
     * @return
     */
	virtual QGraphicsRectItem* asRect() { return 0; }

    /**
     * @brief asQPoolObject Return as QPoolObject
     * @param gItem
     * @return
     */
	static QPoolObject* asQPoolObject(QGraphicsItem* gItem);
};

#endif // QPOOLOBJECT_H

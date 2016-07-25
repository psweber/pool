#ifndef ENGINETICKER_H
#define ENGINETICKER_H

#include <QObject>

#include <QDebug>

namespace engine
{
class PoolEngine;
}


/**
 * @brief The EngineTicker class is responsible for updating the pool engine
 * with a prescribed rate of frames per second. The pool engine takes care
 * of the object movements.
 */
class EngineTicker : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief engine_ The pool engine
     */
	engine::PoolEngine* engine_;

    /**
     * @brief stop_ Flag read in every updating cycle, stops ticker if true
     */
	bool stop_;
	
public:
    /**
     * @brief EngineTicker Constructor requires the pool engine
     * @param engine
     */
    EngineTicker(engine::PoolEngine* engine);
	
	
signals:

public slots:
	void start();
    void stop() { stop_ = true; }
};

#endif // ENGINETICKER_H

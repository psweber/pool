#ifndef SCENEREFRESHER_H
#define SCENEREFRESHER_H

#include <QObject>
#include <QDebug>
class QPoolEngine;

/**
 * @brief The SceneRefresher class is responsible for updating the graphics scene
 * with a prescribed rate of frames per second. To do so, it updates the
 * QPoolObjects with data read from the pool engine.
 */
class SceneRefresher : public QObject
{
    Q_OBJECT
    
private:
	bool stop_;
	int framesPerSecond_;
	QPoolEngine* engine_;

public:
    /**
     * @brief SceneRefresher Construct for given engine
     * @param engine
     */
    SceneRefresher(QPoolEngine* engine);
    
    int framesPerSecond() const { return framesPerSecond_; }
    void framesPerSecond(int fPS) { framesPerSecond_ = fPS; }
    
signals:

public slots:
	void start();
    void stop() { stop_ = true; }
};

#endif // SCENEREFRESHER_H

#include "scenerefresher.h"
#include "qpoolengine.h"
#include <QGraphicsScene>
#include <unistd.h>

#include <QDebug>

SceneRefresher::SceneRefresher(QPoolEngine* engine) :
stop_(true),
framesPerSecond_(5),
engine_(engine)
{}

void SceneRefresher::start()
{
	// If already running do nothing
	if (stop_ == false) { return; }
	stop_ = false;
	
	// Compute sleep time (assuming negligable updating time)
	double sleepTime = 1.0/framesPerSecond_;
	int uSleepTime = 1e6*sleepTime;
	
	// Refresher loop
	while (! stop_)
	{
		engine_->updateObjects();
		engine_->updateScene();
		usleep(uSleepTime);
	}
}

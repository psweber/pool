#include "scenerefresher.h"
#include "qpoolengine.h"
#include <QGraphicsScene>

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
	
	// Refresher loop
	while (! stop_)
	{
		break;
		engine_->tick();
		engine_->updateObjects();
		engine_->updateScene();
	}
}

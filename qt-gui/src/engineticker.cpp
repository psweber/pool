#include "engineticker.h"

#include "qpoolengine.h"

EngineTicker::EngineTicker(QPoolEngine* engine) :
    engine_(engine)
{}

void EngineTicker::start()
{
	// If ticker is already running, do nothing
	if ( stop_ == false ) { return; }
	
	stop_ = false;
	
	// Keep ticking till stop is requested
	while ( ! stop_ )
	{
		engine_->tick();
		engine_->updateObjects();
		//engine_->updateScene();
	}
}

# include "PoolEngine.hpp"

# include <chrono>
# include <unistd.h>

void engine::PoolEngine::ticksPerSecond(int tps)
{
	ticksPerSecond_ = tps;
	secondsPerTick_ = 1.0/tps;
}

void engine::PoolEngine::tick()
{
	// Get current time
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	start = std::chrono::steady_clock::now();

	// Perform tick
	table_.updateKinematics(secondsPerTick_);

	// Get current time and compute elapsed time
	end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	double timeTaken = elapsed_seconds.count();

	// Compute how long to sleep to match given time for tick
	double sleepTime = secondsPerTick_ - timeTaken;
	unsigned int uSleepTime = sleepTime * 1e6;
	usleep(uSleepTime);
}

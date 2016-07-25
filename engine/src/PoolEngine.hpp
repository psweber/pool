# pragma once

# include "PoolTable.hpp"

namespace engine
{

/**
 * @brief The PoolEngine class is the heart of the pool engine library, holding
 * the pool table and ticker information.
 */
class PoolEngine
{
	private:
		PoolTable	table_;
		int			ticksPerSecond_;
		double		secondsPerTick_;
		
	public:
        /**
         * @brief PoolEngine Standard constructor, creates pool table
         */
        PoolEngine() { ticksPerSecond(1); };
        ~PoolEngine() {};
		
		PoolTable& table() { return table_; }
		const PoolTable& table() const { return table_; }
		
		void ticksPerSecond(int tps);
		int ticksPerSecond() const { return ticksPerSecond_; }
		
		double secondsPerTick() const { return secondsPerTick_; }
		
        /**
         * @brief tick Perform one timestep, move objects accordingly, resolve collisions etc.
         */
		void tick();
};
}

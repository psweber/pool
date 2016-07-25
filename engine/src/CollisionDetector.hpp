# pragma once

# include <list>
# include "Objects.hpp"

namespace engine
{
typedef std::pair<std::shared_ptr<MovingObject>,std::shared_ptr<Object>> CollisionPair;
typedef std::list<CollisionPair> CollisionList;

class PoolTable;

/**
 * @brief The CollisionDetector class is base class for different collision detection strategies
 * that can operate either pairwise or on a global basis.
 */
class CollisionDetector
{
	private:
		CollisionList collisionList_;
		PoolTable& table_;
		
	public:
		CollisionDetector(PoolTable& table);
		
        /**
         * @brief collisionList Returns list with all detected collisions
         * @return
         */
		CollisionList& collisionList() { return collisionList_; }

        /**
         * @brief collisionList Returns list with all detected collisions
         * @return
         */
		const CollisionList& collisionList() const { return collisionList_; }

		PoolTable& table() { return table_; }
		const PoolTable& table() const { return table_; }

        /**
         * @brief detect Run detection algorithm to update collisionList
         */
		virtual void detect() = 0;
};

/**
 * @brief The PairwiseBoundingBox class implements a simple, pairwise check
 * if objects have overlapping bounding boxes.
 */
class PairwiseBoundingBox : public CollisionDetector
{
	public:
		PairwiseBoundingBox(PoolTable& table);
		
		virtual void detect();
};

}

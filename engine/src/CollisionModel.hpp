#pragma once

# include <memory>

namespace engine
{

class Object;
class MovingObject;

/**
 * @brief The CollisionType enum lists currently available collision models
 */
enum class CollisionType { ELASTIC };

/**
 * @brief The CollisionModel class is the abstract base for all collision models,
 * requiring them to implement the collide function for computing object property
 * changes implied by a collision.
 */
class CollisionModel
{
    public:
         /**
         * @brief collide Takes two objects that will collided in the past timestep and updates
         * object properties like position and velocity.
         * @param obj1 The first object, always moving
         * @param obj2 The second object, generic type
         * @param deltaT Length of past time step
         */
		virtual void collide
		(
			std::shared_ptr<MovingObject> obj1,
			std::shared_ptr<Object> obj2,
			double deltaT
		) = 0;
};

/**
 * @brief The ElasticCollision class implements simple elastic colisions
 */
class ElasticCollision : public CollisionModel
{
	public:
    /**
    * @brief collide Takes two objects that will collided in the past timestep and updates
    * object properties like position and velocity.
    * @param obj1 The first object, always moving
    * @param obj2 The second object, generic type
    * @param deltaT Length of past time step
    */
		virtual void collide
		(
			std::shared_ptr<MovingObject> obj1,
			std::shared_ptr<Object> obj2,
			double deltaT
		);
};

}

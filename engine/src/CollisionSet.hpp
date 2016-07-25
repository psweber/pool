#pragma once

# include "CollisionModel.hpp"
# include <vector>
# include <memory>
namespace engine
{


enum class ShapeType;
enum class ObjectType;

/**
 * @brief The CollisionSet class defines the collision rule for various collision type, e.g.
 * MovingObject/MovingObject, MovingObject/BoundaryObject, etc. It distuingishes thereby
 * different object shapes, allowing for instance rectangles to collide different than
 * ellipses.
 */
class CollisionSet
{
	private:
		ShapeType shapeI_;
		ShapeType shapeII_;
		ObjectType objectType_;
		
		std::shared_ptr<CollisionModel> model_;
	
	public:
		CollisionSet
		(
			ShapeType shapeI,
			ShapeType shapeII,
			ObjectType objectType,
			CollisionType collisionType
		);
		
        /**
         * @brief collide Perform collision for given objects based on given sets
         * @param sets
         * @param obj1
         * @param obj2
         * @param deltaT
         */
		static void collide
		(
			std::vector<std::shared_ptr<CollisionSet>> sets,
			std::shared_ptr<MovingObject> obj1,
			std::shared_ptr<Object> obj2,
			double deltaT
		);
		
        /**
         * @brief clip Returns true if no collision set for given objects is found
         * @param sets
         * @param obj1
         * @param obj2
         * @return
         */
		static bool clip
		(
			std::vector<std::shared_ptr<CollisionSet>> sets,
			std::shared_ptr<MovingObject> obj1,
			std::shared_ptr<Object> obj2
		);
		
		ShapeType shapeI() const { return shapeI_; }
		ShapeType shapeII() const { return shapeII_; }
		ObjectType objectType() const { return objectType_; }
		std::shared_ptr<CollisionModel> model() { return model_; }
};

}

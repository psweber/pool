# pragma once

# include <memory>
# include <valarray>
# include "Shape.hpp"


namespace engine
{
typedef std::valarray<double> ValArray;
typedef std::shared_ptr<Shape> Shape_ptr;

class PoolTable;

/**
 * @brief The ObjectType enum lists available object types
 */
enum class ObjectType { MOVING, STATIC, BOUNDARY };

/**
 * @brief The Object class is the abstract base for all pool objects. A pool object
 * holds the geometrical information (shape) and kinematic data (velocity).
 */
class Object
{
	private:
        static int nextId_;
        int id_;
        Shape_ptr shape_;
	
	public:
		Object(Shape_ptr shape);
		virtual ~Object();
		
		int id() const { return id_; }
		
		void printBoundingBox() const;
		
		virtual void update() {};
		virtual bool isMoving() const = 0;
		virtual ObjectType type() const = 0;
		
        /**
         * @brief changeShape Request change of shape as given
         * @param toShType
         */
		void changeShape(ShapeType toShType);
		
        /**
         * @brief shape Return the shape
         * @return
         */
		Shape_ptr shape() { return shape_; }

        /**
         * @brief shape Return the shape
         * @return
         */
		const Shape_ptr shape() const { return shape_; }
};

/**
 * @brief The StaticObject class models static objects, that do not
 * change their position when collided by moving objects.
 */
class StaticObject : public Object
{
	public:
		StaticObject(Shape_ptr shape);
		
		virtual ObjectType type() const { return ObjectType::STATIC; }
		virtual bool isMoving() const { return false; };
};

/**
 * @brief The BoundaryObject class models boundary objects as special
 * static objects. This enables different graphical representations
 * as well as collision rules.
 */
class BoundaryObject : public StaticObject
{
	public:
		BoundaryObject(Shape_ptr shape);
		
		virtual ObjectType type() const { return ObjectType::BOUNDARY; }
};

/**
 * @brief The MovingObject class models objects that move on the pool table.
 */
class MovingObject : public Object
{
    typedef std::vector<std::shared_ptr<MovingObject> > MovingObjectVector;
    typedef std::vector<std::shared_ptr<StaticObject> > StaticObjectVector;

	private:
        std::shared_ptr<PoolTable>	table_;
		
        ValArray      velocity_;
        double        timeToMove_;
		

	public:
		MovingObject(Shape_ptr shape);
		
		virtual ObjectType type() const { return ObjectType::MOVING; }
		
		virtual bool isMoving() const { return true; }
		
		virtual void moveForward(double deltaT);
		
		double vx() const { return velocity_[0]; }
		double vy() const { return velocity_[1]; }
		void velocity(ValArray vel) { velocity_ = vel; }
		const ValArray& velocity() { return velocity_; }

};

}

# pragma once

# include <list>
# include "Objects.hpp"


#include <iostream>
namespace engine
{
	
class CollisionDetector;
class CollisionModel;
class CollisionSet;

typedef std::list<std::pair<std::shared_ptr<MovingObject>,std::shared_ptr<Object>>> CollisionList;

typedef std::vector<std::shared_ptr<Object>> ObjectVector;
typedef std::vector<std::shared_ptr<BoundaryObject>> BoundaryObjectVector;
typedef std::vector<std::shared_ptr<MovingObject>> MovingObjectVector;
typedef std::vector<std::shared_ptr<StaticObject>> StaticObjectVector;

/**
 * @brief The PoolTable class holds all data related to the table, its objects and its collision rules.
 */
class PoolTable
{

	private:
		BoundaryObjectVector boundaryObjects_;
		MovingObjectVector movingObjects_;
		StaticObjectVector staticObjects_;
		
        std::shared_ptr<CollisionDetector> detector_;
		
		std::vector<std::shared_ptr<CollisionSet>> collisionSets_;
		
		template <typename T>
		void deleteObjectFromVector
		(
			std::shared_ptr<Object> obj,
			T& vec
		);
		
	public:
		PoolTable();
		
        /**
         * @brief collisionSets Returns all defined collision sets
         * @return
         */
		std::vector<std::shared_ptr<CollisionSet>> collisionSets() { return collisionSets_; }
		
		BoundaryObjectVector& boundaryObjects() { return boundaryObjects_; }
		const BoundaryObjectVector& boundaryObjects() const { return boundaryObjects_; }

		MovingObjectVector& movingObjects() { return movingObjects_; }
		const MovingObjectVector& movingObjects() const { return movingObjects_; }
		
        StaticObjectVector& staticObjects() { return staticObjects_; }
        const StaticObjectVector& staticObjects() const { return staticObjects_; }
		
        /**
         * @brief createObject Templated generator of objects with given center and shape
         * Requires to set shape information aftwards
         */
        template <typename T>
		std::shared_ptr<T> createObject
		(
			std::vector<std::shared_ptr<T>>& objectList,
			ValArray& center,
			ShapeType shType
		);

        /**
         * @brief createBoundary Create table boundary elements with thickness relative to width/height
         * @param width
         * @param height
         * @param relThickness
         */
		void createBoundary(double width, double height, double relThickness = 0.02);

        /**
         * @brief createBoundaryObject Create single boundary object
         * @param center
         * @param width
         * @param height
         */
		void createBoundaryObject(ValArray center, double width, double height);
		
        /**
         * @brief updateKinematics Perform one timestep and move objects accordingly
         * @param deltaT
         */
		void updateKinematics(double deltaT);
		
        /**
         * @brief deleteObject Delete given object
         * @param obj
         */
		void deleteObject(std::shared_ptr<Object> obj);
		
        /**
         * @brief printBoundingBoxes Print bounding boxes to stdout
         */
		void printBoundingBoxes() const;
};

template <typename T>
std::shared_ptr<T>
PoolTable::createObject
(
	std::vector<std::shared_ptr<T>>& objectList,
	ValArray& center,
	ShapeType shType
)
{
	std::shared_ptr<Shape> shape;
	
	switch (shType)
	{
		case ShapeType::RECTANGLE:
			shape = std::shared_ptr<Shape>( new RectangleShape(center) );
			break;
			
		case ShapeType::ELLIPSE:
			shape = std::shared_ptr<Shape>( new EllipseShape(center) );
			break;
		
		default:
			std::cerr << "ERROR: Incorrect shapeType in PoolTable::createObject(...)\n";
	}
	
	std::shared_ptr<T> typeObj
	(
		new T(shape)
	);
	
	objectList.push_back(typeObj);
	
	return typeObj;
}


template <typename T>
void PoolTable::deleteObjectFromVector
(
	std::shared_ptr<Object> obj,
	T& vec
)
{
	// Get position
	auto it = std::find(vec.begin(), vec.end(), obj);
	
	// Remove
	if(it != vec.end())
	{
		std::cout << "Remove object id " << (*it)->id() << " from object vector\n";
		vec.erase(it);
	}
}

}

# include "CollisionDetector.hpp"
# include "PoolTable.hpp"
# include "CollisionSet.hpp"
# include <algorithm>
# include <iostream>

engine::CollisionDetector::CollisionDetector(PoolTable& table) :
table_(table)
{}

engine::PairwiseBoundingBox::PairwiseBoundingBox(PoolTable& table) :
CollisionDetector(table)
{}

void engine::PairwiseBoundingBox::detect()
{
	collisionList().clear();
	
	// Walk through all object/object combinations and check for
	// intersecting bounding boxes
	MovingObjectVector& movingObjects = table().movingObjects();
	BoundaryObjectVector& boundaryObjects = table().boundaryObjects();
	StaticObjectVector& staticObjects = table().staticObjects();
	
	for (unsigned int i=0; i<movingObjects.size(); i++)
	{
		std::shared_ptr<MovingObject> movObj(movingObjects[i]);
		
		// Check for MovingObject/MovingObject intersections
		for (unsigned int j=i+1; j<movingObjects.size(); j++)
		{
			std::shared_ptr<MovingObject> movObj2(movingObjects[j]);
			
			if
			(
				! CollisionSet::clip(table().collisionSets(),movObj, movObj2)
			 && 
				RectangleShape::rectanglesOverlap
				(
					movObj->shape()->boundingBox(),
					movObj2->shape()->boundingBox()
				)
			)
			{
				collisionList().push_back
				(
					CollisionPair(movObj,movObj2)
				);
			}
		}
				
		// Check for MovingObject/BoundaryObject intersections
		for (auto boundObj : boundaryObjects)
		{
			if
			(
				! CollisionSet::clip(table().collisionSets(),movObj, boundObj)
			 && 
				RectangleShape::rectanglesOverlap
				(
					movObj->shape()->boundingBox(),
					boundObj->shape()->boundingBox()
				)
			)
			{
				collisionList().push_back
				(
					CollisionPair(movObj,boundObj)
				);
			}
		}
				
		// Check for MovingObject/StaticObject intersections
		for (auto statObj : staticObjects)
		{
			if
			(
				! CollisionSet::clip(table().collisionSets(),movObj, statObj)
			 && 
				RectangleShape::rectanglesOverlap
				(
					movObj->shape()->boundingBox(),
					statObj->shape()->boundingBox()
				)
			)
			{
				collisionList().push_back
				(
					CollisionPair(movObj,statObj)
				);
			}
		}
	}
}


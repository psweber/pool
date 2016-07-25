# include "PoolTable.hpp"
# include "CollisionDetector.hpp"
# include "CollisionModel.hpp"
# include "CollisionSet.hpp"

# include <iostream>

engine::PoolTable::PoolTable()
:
detector_(new PairwiseBoundingBox(*this))
{}

void engine::PoolTable::updateKinematics(double deltaT)
{
	// Move all moving objects without constraint
	for (auto& movObj : movingObjects_)
	{
		movObj->moveForward(deltaT);
	}
	
	// Update collision list
	detector_->detect();
    CollisionList& cList(detector_->collisionList());
    
    // Iterate to resolve collisions (only required for high object densities)
    for (int counter=0; counter<10; counter++)
	{
		if ( cList.empty() ) { break; }
		
		// Walk through list with colliding pairs and resolve pairwise
        for (auto colPair: cList)
		{
			CollisionSet::collide(collisionSets(),colPair.first,colPair.second,deltaT);
		}
		
		// Update collision list
		detector_->detect();
        cList = detector_->collisionList();
	}
	
}

void engine::PoolTable::createBoundaryObject(ValArray center, double width, double height)
{
    // Create rectangle with given center
	std::shared_ptr<BoundaryObject> boundObj
	(
		createObject<BoundaryObject>
		(
			boundaryObjects(),
			center,
			ShapeType::RECTANGLE
		)
	);
	
	// Adjust size
	std::shared_ptr<RectangleShape> rectShape
    (
        std::static_pointer_cast<RectangleShape>(boundObj->shape())
    );
    
    rectShape->size(width,height);
}

void engine::PoolTable::createBoundary(double width, double height, double relThickness)
{
	// Absolute thickness of boundary objects
    int min = (width < height ? width : height);
    double thickness = relThickness * min;

    // Top, right, bottom, left boundary
    createBoundaryObject(ValArray{0.5*width,0.5*thickness}     , width, thickness);
    createBoundaryObject(ValArray{width-0.5*thickness,0.5*height} , thickness      , height);
    createBoundaryObject(ValArray{0.5*width,height-0.5*thickness}, width, thickness);
    createBoundaryObject(ValArray{0.5*thickness,0.5*height}     , thickness      , height);
}

void engine::PoolTable::printBoundingBoxes() const
{
	for (auto obj : movingObjects())
	{
		obj->printBoundingBox();
	}
	
	for (auto obj : boundaryObjects())
	{
		obj->printBoundingBox();
	}
}

void engine::PoolTable::deleteObject(std::shared_ptr<Object> obj)
{
	switch (obj->type())
	{
		case ObjectType::BOUNDARY:
			deleteObjectFromVector(obj,boundaryObjects_);
			break;
			
		case ObjectType::STATIC:
			deleteObjectFromVector(obj,staticObjects_);
			break;
			
		case ObjectType::MOVING:
			deleteObjectFromVector(obj,movingObjects_);
			break;
	}
}

void engine::PoolTable::createCollisionSet(ObjectType objType, CollisionType colType)
{
	for ( int i=1; i<3; i++)
	{
		ShapeType shTy1 = (ShapeType) i;
		
		for ( int j=1; j<3; j++)
		{
			ShapeType shTy2 = (ShapeType) j;
		
			createCollisionSet(shTy1,shTy2,objType,colType);
		}
	}
}

void engine::PoolTable::createCollisionSet
(
	ShapeType shTypeI,
	ShapeType shTypeII,
	ObjectType objType,
	CollisionType colType
)
{
	collisionSets_.push_back
	(
		std::shared_ptr<CollisionSet>
		(
			new CollisionSet(shTypeI,shTypeII,objType,colType)
		)
	);
}


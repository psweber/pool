# include "CollisionModel.hpp"

# include "Objects.hpp"
# include "Utils.hpp"

# include <iostream>

void engine::ElasticCollision::collide
(
	std::shared_ptr<MovingObject> obj1,
	std::shared_ptr<Object> obj2,
	double deltaT
)
{
	// See if second object is also moving
	std::shared_ptr<MovingObject> mObj2(0);
		
	if (obj2->type() == ObjectType::MOVING)
	{
		mObj2 = std::static_pointer_cast<MovingObject>(obj2);
	}

	// Revert objects to last time step
	obj1->moveForward(-deltaT);

	if (obj2->type() == ObjectType::MOVING)
	{
		mObj2->moveForward(-deltaT);
	}
	
	
	// Get the bounding boxes
	std::shared_ptr<RectangleShape> bb1(obj1->shape()->boundingBox());
	std::shared_ptr<RectangleShape> bb2(obj2->shape()->boundingBox());
	
	// Relative velocities ( sees second object as static )
	double vxRel = -obj1->vx(); // + obj2->vx();
	double vyRel = -obj1->vy(); // + obj2->vy();
	
	if (obj2->type() == ObjectType::MOVING)
	{
		vxRel += mObj2->vx();
		vyRel += mObj2->vy();
	}


	// Difference of center positions
	double cxRel = bb2->cx() - bb1->cx();
	double cyRel = bb2->cy() - bb1->cy();
	
	// Minimal padding between centers
	double padx = 0.5*(bb2->width() + bb1->width());
	double pady = 0.5*(bb2->height() + bb1->height());
	
	// Time till collision in x direction
	double tx;
	if (cxRel > 0) { tx = (cxRel-padx)/stabilize(-vxRel); }
	else { tx = (-cxRel-padx)/stabilize(vxRel); }
	
	// Time till collision in y direction
	double ty;
	if (cyRel > 0) { ty = (cyRel-pady)/stabilize(-vyRel); }
	else { ty = (-cyRel-pady)/stabilize(vyRel); }
	
	
	
	// Compute collision normal
	ValArray n1({0,0});
	
	if // collision in x direction
	(
		((ty<0) && (tx>0)) || ((ty>0) && (tx>0) && (tx<ty))
	)
	{ n1[0] = vxRel; }
	
	else if // collision in y direction
	(
		((tx<0) && (ty>0)) || ((ty>0) && (tx>0) && (ty<tx))
	)
	{ n1[1] = vyRel; }
	
	// Objects already in collision
	else
	{
		// Decouple objects
		obj1->moveForward(-deltaT);
		
		// Warn
		std::cout << "WARNING: Could not separate objects " << obj1->id()
			<< " and " << obj2->id() << "!" << std::endl;
		return;
	}
	
	// Scale by magnitude
	n1 /= std::sqrt( n1[0]*n1[0] + n1[1]*n1[1] );
	
	
	// Update velocity (one moving object)
	if (obj2->type() != ObjectType::MOVING)
	{
		ValArray vel1(obj1->velocity());
		double v1dotn ( vel1[0]*n1[0] + vel1[1]*n1[1] );
		ValArray vel1n( v1dotn*n1 );
		ValArray vel1t( vel1 - vel1n );
		vel1 -= 2.0*vel1n;
		obj1->velocity(vel1);
	}
	
	// Update velocity (two moving objects)
	else
	{
		// Update velocity of first object
		ValArray n2(-n1);
		
		ValArray vel1(obj1->velocity());
		ValArray vel2(mObj2->velocity());
		
		double v1dotn ( vel1[0]*n1[0] + vel1[1]*n1[1] );
		double v2dotn ( vel2[0]*n2[0] + vel2[1]*n2[1] );
		
		ValArray vel1n( v1dotn*n1 );
		ValArray vel2n( v2dotn*n2 );
		
		ValArray vel1t( vel1 - vel1n );
		ValArray vel2t( vel2 - vel2n );
		
		vel1 += -vel1n +vel2n;
		vel2 += -vel2n +vel1n;
		
		obj1->velocity(vel1);
		mObj2->velocity(vel2);
		
	}
}

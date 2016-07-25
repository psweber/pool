# include "Objects.hpp"
# include <iostream>

int engine::Object::nextId_ = 0;

engine::Object::Object(Shape_ptr shape)
:
id_(nextId_++),
shape_(shape)
{
}

engine::Object::~Object()
{}

engine::StaticObject::StaticObject(Shape_ptr shape)
:
Object(shape)
{}

void engine::Object::changeShape(ShapeType toShType)
{
	ValArray c(shape()->center());
	
	switch(toShType)
	{
		case ShapeType::RECTANGLE:
			shape_ = Shape_ptr(new RectangleShape(c));
			break;
			
		case ShapeType::ELLIPSE:
			shape_ = Shape_ptr(new EllipseShape(c));
			break;
		
		default:
			break;
	}
}

engine::BoundaryObject::BoundaryObject(Shape_ptr shape)
:
StaticObject(shape)
{}

engine::MovingObject::MovingObject(Shape_ptr shape)
:
Object(shape),
table_(0),
velocity_(ValArray(shape->center().size())),
timeToMove_(0)
{
	velocity_ = 0;
}

void engine::MovingObject::moveForward(double deltaT)
{
    shape()->center() += velocity_ * deltaT;
}

void engine::Object::printBoundingBox() const
{
	std::cout << "BB " << id() << ": x,y " << shape()->boundingBox()->cx()
		<< ", " << shape()->boundingBox()->cy()
		<< ", w,h " << shape()->boundingBox()->width()
		<< ", "<< shape()->boundingBox()->height() << std::endl;
}





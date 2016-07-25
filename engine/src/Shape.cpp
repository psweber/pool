# include "Shape.hpp"


engine::Shape::Shape(int numDimensions)
:
center_(numDimensions,0)
{}

engine::Shape::Shape(const Shape& shape)
:
center_(shape.center())
{}

engine::Shape::Shape(const ValArray& center)
:
center_(center)
{}

engine::RectangleShape::RectangleShape(double width, double height)
:
Shape(2),
width_(width),
height_(height)
{}

engine::RectangleShape::RectangleShape(double width, double height, const ValArray& center)
:
Shape(center),
width_(width),
height_(height)
{}

engine::RectangleShape::RectangleShape(const ValArray& center)
:
Shape(center),
width_(0),
height_(0)
{}

engine::RectangleShape::RectangleShape(const RectangleShape& rect)
:
Shape(rect),
width_(rect.width()),
height_(rect.height())
{}

engine::RectangleShape::RectangleShape
(
	double size,
	double aspectRatio,
	bool horizontal = true
)
{
	width_ = size;
	height_ = size;
	
	if (horizontal)
	{
		width_ *= aspectRatio;
	}
	else
	{
		height_ *= aspectRatio;
	}
}

std::shared_ptr<engine::RectangleShape> engine::RectangleShape::boundingBox() const
{
	std::shared_ptr<RectangleShape> rect
	(
		new RectangleShape(*this)
	);
	
	return rect;
}

bool engine::RectangleShape::rectanglesOverlap
(
	std::shared_ptr<RectangleShape> rect1,
	std::shared_ptr<RectangleShape> rect2
)
{
	bool xDistOK = true;
	bool yDistOK = true;
	
	// Check x distance
	if
	(
		fabs( rect1->cx()-rect2->cx() ) <
			0.5*( rect1->width()+rect2->width() )
	)
	{
		xDistOK = false;
	}
	
	// Check y distance
	if
	(
		fabs( rect1->cy()-rect2->cy() ) <
			0.5*( rect1->height()+rect2->height() )
	)
	{
		yDistOK = false;
	}
	
	// Intersection only if all distances are too small
	if (xDistOK || yDistOK)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}


engine::EllipseShape::EllipseShape
(
	double size,
	double aspectRatio,
	bool horizontal = true
)
{
	axisX_ = size;
	axisY_ = size;
	
	if (horizontal)
	{
		axisX_ *= aspectRatio;
	}
	else
	{
		axisY_ *= aspectRatio;
	}
}

engine::EllipseShape::EllipseShape(const ValArray& center)
:
Shape(center),
axisX_(0),
axisY_(0)
{}

std::shared_ptr<engine::RectangleShape> engine::EllipseShape::boundingBox() const
{
	std::shared_ptr<RectangleShape> rect
	(
		new RectangleShape(2.0*axisX_,2.0*axisY_,center())
	);
	
	return rect;
}

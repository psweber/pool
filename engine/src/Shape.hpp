# pragma once

# include <valarray>
# include <memory>

namespace engine
{
typedef std::valarray<double> ValArray;

class RectangleShape;

/**
 * @brief The ShapeType enum lists currently available shape types
 */
enum class ShapeType { NONE, RECTANGLE, ELLIPSE };

/**
 * @brief The Shape class is the abstract base for different object shapes
 * like rectangles or ellipses.
 */
class Shape
{
	private:
        ValArray center_;
		
	public:
		Shape(int numDimensions = 2);
        Shape(const ValArray& center);
        Shape(const Shape& shape);
		
        void center(const ValArray& center) { center_ = center; }
        ValArray& center() { return center_; }
        const ValArray& center() const { return center_; }
        
        double cx() const { return center_[0]; }
        double cy() const { return center_[1]; }
        
		virtual ShapeType type() const = 0;
		
        virtual std::shared_ptr<RectangleShape> boundingBox() const = 0;
};

/**
 * @brief The RectangleShape class implements a rectangular shape and also
 * the static function for checking wether two rectangles overlap.
 */
class RectangleShape : public Shape
{
	private:
		double width_;
		double height_;
	
	public:
		RectangleShape(const ValArray& center);
		RectangleShape(double width, double height);
		RectangleShape(double width, double height, const ValArray& center);
		RectangleShape(const RectangleShape& rect);
		RectangleShape(double size, double aspectRatio, bool horizontal);
		
		virtual ShapeType type() const { return ShapeType::RECTANGLE; };
		
        virtual std::shared_ptr<RectangleShape> boundingBox() const;
        
		void size(double w, double h) { width_ = w; height_ = h; }
		
		double width() const { return width_; }
		void width(double w) { width_ = w; }
		
		double height() const { return height_; }
		void height(double h) { height_ = h; }
		
		static bool rectanglesOverlap
		(
			std::shared_ptr<RectangleShape> rect1,
			std::shared_ptr<RectangleShape> rect2
		);
};

/**
 * @brief The EllipseShape class implements an elliptic shape
 */
class EllipseShape : public Shape
{
	private:
		double axisX_;
		double axisY_;
	
	public:
		EllipseShape(double size, double aspectRatio, bool horizontal);
		EllipseShape(const ValArray& center);
	
		virtual ShapeType type() const { return ShapeType::ELLIPSE; };
		
        virtual std::shared_ptr<RectangleShape> boundingBox() const;
        
		void size(double x, double y) { axisX_ = x; axisY_ = y; }
		
		void axisX(double x) { axisX_ = x; }
		double axisX() const { return axisX_; }
		
		void axisY(double y) { axisY_ = y; }
		double axisY() const { return axisY_; }
		
};


}

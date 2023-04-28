#include "Point.h"

namespace Com_Methods
{
	Point::Point(double x = 0, double y = 0, double z = 0) : _x(x), _y(y), _z(z) {};
	double Point::x() const { return _x; };
	double Point::y() const { return _y; };
	double Point::z() const { return _z; };
}
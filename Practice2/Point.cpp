#include "Point.h"

Point::Point(double x = 0, double y = 0, double z = 0) : _x(x), _y(y), _z(z) {};
double Point::x() { return _x; };
double Point::y() { return _y; };
double Point::z() { return _z; };
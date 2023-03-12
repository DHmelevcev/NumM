#pragma once

#include <vector>
#include "Point.h"

class Spline
{
protected:

	std::vector<Point> knots;

public:
	
	virtual void update(std::vector<Point>& points, std::vector<double>& values) = 0;
	virtual std::vector<double> get_value(Point& point) = 0;
};
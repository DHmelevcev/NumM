#pragma once

#include "Spline.h"

class Cubic_Interpolation_Spline_1d final : public Spline
{
private:
	
	std::vector<double> a, b, c, d;

public:

	void update(std::vector<Point>& points, std::vector<double>& values) override;
	std::vector<double> get_value(Point& point) override;
};
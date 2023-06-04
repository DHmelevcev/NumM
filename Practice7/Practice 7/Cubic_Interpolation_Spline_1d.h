#pragma once
#include "Spline.h"

class Cubic_Interpolation_Spline_1d final : public Spline
{
private:
	
	std::vector<double> a, b, c, d;

public:

	void update(std::vector<double>& points, std::vector<double>& values) override;
	double get_value(double point, int derivitive = 0) override;
};
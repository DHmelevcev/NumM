#pragma once
#ifndef Cubic_Interpolation_Spline_1d_h
#define Cubic_Interpolation_Spline_1d_h

#include "Spline.h"

class Cubic_Interpolation_Spline_1d final : public Spline
{
private:
	
	std::vector<double> a, b, c, d;

public:

	void update(const std::vector<double>& points, const std::vector<double>& values) override;
	double get_value(const double point, int derivitive = 0) const override;
};

#endif
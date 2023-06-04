#pragma once
#include <vector>
#include <complex>

class Spline
{
protected:

	std::vector<double> knots;

public:
	
	virtual void update(std::vector<double>& points, std::vector<double>& values) = 0;
	virtual double get_value(double point, int derivitive) = 0;
};
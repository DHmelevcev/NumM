#pragma once
#include <vector>

struct oscillation
{
	oscillation(size_t knots, double duration) : knots(knots), duration(duration)
	{
		X.resize(knots);
		Y.resize(knots);
	}
	
	size_t knots;
	double duration;
	std::vector<double> X;
	std::vector<double> Y;
};
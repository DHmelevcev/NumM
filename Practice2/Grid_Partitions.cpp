#include "Grid_Partitions.h"

std::vector<Point> grid_partitions_generator(double a, double b, int parts, double charge)
{
	if (fabs(a - b) < 1e-7 || parts < 2 || charge < 1e-7)
		throw std::exception("Invalid input");

	std::vector<Point> res;
	double x = a;
	res.push_back(Point(x, 0.0, 0.0));

	double s = 1;
	for (int i = 1; i < parts; i++)
		s += pow(charge, i);
	double step = (b - a) / s;

	for (int i = 0; i < parts; i++)
		res.push_back(Point(x += step * pow(charge, i), 0.0, 0.0));

	return res;
}
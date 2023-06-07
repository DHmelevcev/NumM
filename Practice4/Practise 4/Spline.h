#pragma once
#ifndef Spline_h
#define Spline_h

#include <vector>

//абстрактный класс сплайна
class Spline
{
protected:

	std::vector<double> knots;

public:
	//обновить сплайн
	virtual void update(const std::vector<double>& points, const std::vector<double>& values) = 0;
	//вычислить значение сплайна в точке P
	virtual double get_value(const double point, int derivitive) const = 0;
};

#endif
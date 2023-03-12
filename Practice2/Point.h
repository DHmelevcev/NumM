#pragma once

class Point
{
private:
	double _x, _y, _z;

public:
	
	Point(double x, double y, double z);
	double x();
	double y();
	double z();
};
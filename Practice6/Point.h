#pragma once

namespace Com_Methods
{
	class Point
	{
	public:

		Point(double x, double y, double z);
		double x() const;
		double y() const;
		double z() const;

	private:

		double _x, _y, _z;
	};
}
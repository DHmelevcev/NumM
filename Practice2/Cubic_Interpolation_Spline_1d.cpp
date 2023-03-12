#include "Cubic_Interpolation_Spline_1d.h"

void Cubic_Interpolation_Spline_1d::update(std::vector<Point>& points, std::vector<double>& values)
{
	knots.clear();
	for (auto& elem : points)
		knots.push_back(elem);

	int Num_Segments = knots.size() - 1;
	double h_cur = 0, h_next = 0;

	a.resize(Num_Segments);
	b.resize(Num_Segments);
	c.resize(Num_Segments);
	d.resize(Num_Segments);
	std::vector<double> f(Num_Segments - 1);

	for (int i = 0; i < Num_Segments - 1; i++)
	{
		h_cur = knots[i + 1].x() - knots[i].x();
		h_next = knots[i + 2].x() - knots[i + 1].x();

		//main diagonal
		b[i] = 2 * (h_cur + h_next);

		//lower diagonal
		a[i] = h_cur;

		//upper diagonal
		d[i] = h_next;

		//right part
		f[i] = 3.0 * ((values[i + 2] - values[i + 1]) / h_next - (values[i + 1] - values[i]) / h_cur);
	}

	for (int i = 1; i < Num_Segments - 1; i++)
	{
		b[i] -= a[i] / b[i - 1] * d[i - 1];
		f[i] -= a[i] / b[i - 1] * f[i - 1];
	}

	c[Num_Segments - 1] = f[Num_Segments - 2] / b[Num_Segments - 2];
	for (int i = Num_Segments - 2; i > 0; i--)
	{
		c[i] = (f[i - 1] - c[i + 1] * d[i - 1]) / b[i - 1];
	}
	c[0] = 0.0;

	for (int i = 0; i < Num_Segments - 1; i++, h_cur = knots[i + 1].x() - knots[i].x())
	{
		a[i] = values[i];
		b[i] = (values[i + 1] - values[i]) / h_cur - (2.0 * c[i] + c[i + 1]) * h_cur / 3.0;
		d[i] = (c[i + 1] - c[i]) / h_cur / 3.0;
	}

	a[Num_Segments - 1] = values[Num_Segments - 1];
	b[Num_Segments - 1] = (values[Num_Segments] - values[Num_Segments - 1]) / h_cur - 2.0 * c[Num_Segments - 1] * h_cur / 3.0;
	d[Num_Segments - 1] = - c[Num_Segments - 1] / h_cur / 3.0;
}

std::vector<double> Cubic_Interpolation_Spline_1d::get_value(Point& point)
{
	std::vector<double> res;
	double eps(1e-7);

	for (int i = 0; i < knots.size() - 1; i++)
	{
		if (point.x() > knots[i].x() && point.x() < knots[i + 1].x() || fabs(point.x() - knots[i].x()) < eps || fabs(point.x() - knots[i + 1].x()) < eps)
		{
			double diff = point.x() - knots[i].x();
			res.push_back(a[i] + b[i] * diff + c[i] * pow(diff, 2) + d[i] * pow(diff, 3));
			res.push_back(b[i] + 2.0 * c[i] * diff + 3.0 * d[i] * pow(diff, 2));
			res.push_back(2.0 * c[i] + 6.0 * d[i] * diff);
			return res;
		}
	}

	throw std::exception("Point is not found...");
}
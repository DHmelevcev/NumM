//#include <functional>
#include "Smoothing_Spline_1d.h"

//�����������: SMOOTH - �������� �����������
Smoothing_Spline_1d::Smoothing_Spline_1d(const double& SMOOTH) : SMOOTH(SMOOTH) {}

//������� �� ������ ������� [-1, 1]: 
//Seg_Num - ����� ��������, � - ��������
double Smoothing_Spline_1d::Transition_To_Master_Element(int Seg_Num, const double &X) const
{
	return 2.0 * (X - knots[Seg_Num]) / (knots[Seg_Num + 1] - knots[Seg_Num]) - 1.0;
}

//�������� ������� �� [-1, 1]:
//Number - ����� �������, Ksi - ���������� �� ������-��������
double Smoothing_Spline_1d::Basis_Function(int Number, const double &Ksi) const
{
	switch (Number)
	{
		case 1:  {return 0.5*(1 - Ksi); break; }
		case 2:  {return 0.5*(1 + Ksi); break; }
		default: {throw std::exception("Error in the basis function number..."); break; }
	}
}

//����������� �������� ������� �� [-1, 1]:
//Number - ����� �������, Ksi - ���������� �� ������-��������
double Smoothing_Spline_1d::Der_Basis_Function(int Number, const double &Ksi) const
{
	switch (Number)
	{
		case 1:  {return -0.5; break; }
		case 2:  {return  0.5; break; }
		default: {throw std::exception("Error in the basis function derivative number..."); break; }
	}
}

//�������� ������
void Smoothing_Spline_1d::update(const std::vector<double>& points, const std::vector<double>& values)
{
	//���������� ������ ����� �������
	this->knots.clear();
	for (auto & elem : points) this->knots.push_back(elem);

	//����� �������� ���������
	int Num_Segments = knots.size() - 1;

	//������������ ���������� �� ������
	alpha.resize(Num_Segments + 1);

	//��������� �������
	std::vector <double> a, b, c;
	a.resize(Num_Segments + 1); b.resize(Num_Segments + 1); c.resize(Num_Segments + 1);
		
	//��������� ��� ��������������� ����: 
	//Num_Segment - ����� �������, P - �����, Val - �������� ��������� ������� � �����, w - ���  
	auto Assembling = [&](int i, const double &P, const double &Val, const double &w)
	{
		//������� �� ������-�������
		double Ksi = Transition_To_Master_Element(i, P);
		//���������� �������� �������� ������� �� ������-��������
		double f1 = Basis_Function(1, Ksi);
		double f2 = Basis_Function(2, Ksi);

		//�������� ������� � ������� ����
		b[i]     += (1.0 - SMOOTH) * w * f1 * f1;
		b[i + 1] += (1.0 - SMOOTH) * w * f2 * f2;
		a[i + 1] += (1.0 - SMOOTH) * w * f1 * f2;
		c[i]	 += (1.0 - SMOOTH) * w * f2 * f1;
		//�������� ������� � ������ ������ ����� ����
		alpha[i]     += (1.0 - SMOOTH) * w * f1 * Val;
		alpha[i + 1] += (1.0 - SMOOTH) * w * f2 * Val;
	};

	//������ ���� �� �����: ����� ������� �� ������� �������� ���������
	for (int i = 0; i < Num_Segments; i++)
	{
		//���������� ���� ����� � ����
		double W = 1.0;
		//if (values[i] > 5 || values[i] < 5) W = 0.33;
		Assembling(i, this->knots[i], values[i], W);
		Assembling(i, this->knots[i + 1], values[i + 1], W);

		//����� �� ����������� �� ������ �����������
		double h = knots[i + 1] - knots[i];
		b[i]	 += 1.0 / h * SMOOTH;
		b[i + 1] += 1.0 / h * SMOOTH;
		a[i + 1] -= 1.0 / h * SMOOTH;
		c[i]	 -= 1.0 / h * SMOOTH;
	}

	//����� ��������: ������ ���
	for (int j = 1; j < Num_Segments + 1; j++)
	{
		//���������
		b[j] -= a[j] / b[j - 1] * c[j - 1];
		//������ �����         
		alpha[j] -= a[j] / b[j - 1] * alpha[j - 1]; 
	}

	//����� ��������: �������� ���
	alpha[Num_Segments] /= b[Num_Segments];
	for (int j = Num_Segments - 1; j >= 0; j--)
		alpha[j] = (alpha[j] - alpha[j + 1] * c[j]) / b[j];
}

//��������� �������� ������� � ����� X
double Smoothing_Spline_1d::get_value(const double X, int derivitive) const
{
	double res = 0;
	double eps = 1e-7;

	if (derivitive == 0 || derivitive == 1)
	{
		//����� �������, �������� ����������� �����
		for (int i = 0; i < knots.size() - 1; i++)
		{
			if (X > knots[i] && X < knots[i + 1] ||
				fabs(X - knots[i]) < eps ||
				fabs(X - knots[i + 1]) < eps)
			{
				//����� �������
				double h = knots[i + 1] - knots[i];
				//������� �� ������-�������, Ksi - ���������� �� ������-��������
				double Ksi = Transition_To_Master_Element(i, X);
				//��������� �������� ������� � ����������� �� �������� ��������
				switch (derivitive)
				{
				case(0):
					res = alpha[i] * Basis_Function(1, Ksi) + alpha[i + 1]  * Basis_Function(2, Ksi);
					break;
				case(1):
					res = (alpha[i] * Der_Basis_Function(1, Ksi) + alpha[i + 1] * Der_Basis_Function(2, Ksi)) * 2.0 / h;
					break;
				}

				return res;
			}
		}
	}

	return res;
}

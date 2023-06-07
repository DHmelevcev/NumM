//#include <functional>
#include "Smoothing_Spline_1d.h"

//конструктор: SMOOTH - параметр сглаживания
Smoothing_Spline_1d::Smoothing_Spline_1d(const double& SMOOTH) : SMOOTH(SMOOTH) {}

//переход на мастер элемент [-1, 1]: 
//Seg_Num - номер сегмента, Х - абсцисса
double Smoothing_Spline_1d::Transition_To_Master_Element(int Seg_Num, const double &X) const
{
	return 2.0 * (X - knots[Seg_Num]) / (knots[Seg_Num + 1] - knots[Seg_Num]) - 1.0;
}

//базисные функции на [-1, 1]:
//Number - номер функции, Ksi - координата на мастер-элементе
double Smoothing_Spline_1d::Basis_Function(int Number, const double &Ksi) const
{
	switch (Number)
	{
		case 1:  {return 0.5*(1 - Ksi); break; }
		case 2:  {return 0.5*(1 + Ksi); break; }
		default: {throw std::exception("Error in the basis function number..."); break; }
	}
}

//производные базисных функций на [-1, 1]:
//Number - номер функции, Ksi - координата на мастер-элементе
double Smoothing_Spline_1d::Der_Basis_Function(int Number, const double &Ksi) const
{
	switch (Number)
	{
		case 1:  {return -0.5; break; }
		case 2:  {return  0.5; break; }
		default: {throw std::exception("Error in the basis function derivative number..."); break; }
	}
}

//обновить сплайн
void Smoothing_Spline_1d::update(const std::vector<double>& points, const std::vector<double>& values)
{
	//обновление списка точек сплайна
	this->knots.clear();
	for (auto & elem : points) this->knots.push_back(elem);

	//число отрезков разбиения
	int Num_Segments = knots.size() - 1;

	//коэффициенты разложения по базису
	alpha.resize(Num_Segments + 1);

	//диагонали матрицы
	std::vector <double> a, b, c;
	a.resize(Num_Segments + 1); b.resize(Num_Segments + 1); c.resize(Num_Segments + 1);
		
	//процедура для ассемблирования СЛАУ: 
	//Num_Segment - номер отрезка, P - точка, Val - значение табличной функции в точке, w - вес  
	auto Assembling = [&](int i, const double &P, const double &Val, const double &w)
	{
		//переход на мастер-элемент
		double Ksi = Transition_To_Master_Element(i, P);
		//вычисление значений базисных функций на мастер-элементе
		double f1 = Basis_Function(1, Ksi);
		double f2 = Basis_Function(2, Ksi);

		//внесение вкладов в матрицу СЛАУ
		b[i]     += (1.0 - SMOOTH) * w * f1 * f1;
		b[i + 1] += (1.0 - SMOOTH) * w * f2 * f2;
		a[i + 1] += (1.0 - SMOOTH) * w * f1 * f2;
		c[i]	 += (1.0 - SMOOTH) * w * f2 * f1;
		//внесение вкладов в вектор правой части СЛАУ
		alpha[i]     += (1.0 - SMOOTH) * w * f1 * Val;
		alpha[i + 1] += (1.0 - SMOOTH) * w * f2 * Val;
	};

	//сборка СЛАУ по сетке: сумма вкладов от каждого сегмента разбиения
	for (int i = 0; i < Num_Segments; i++)
	{
		//добавление узла сетки в СЛАУ
		double W = 1.0;
		//if (values[i] > 5 || values[i] < 5) W = 0.33;
		Assembling(i, this->knots[i], values[i], W);
		Assembling(i, this->knots[i + 1], values[i + 1], W);

		//вклад от сглаживания по первой производной
		double h = knots[i + 1] - knots[i];
		b[i]	 += 1.0 / h * SMOOTH;
		b[i + 1] += 1.0 / h * SMOOTH;
		a[i + 1] -= 1.0 / h * SMOOTH;
		c[i]	 -= 1.0 / h * SMOOTH;
	}

	//метод прогонки: прямой ход
	for (int j = 1; j < Num_Segments + 1; j++)
	{
		//диагональ
		b[j] -= a[j] / b[j - 1] * c[j - 1];
		//правая часть         
		alpha[j] -= a[j] / b[j - 1] * alpha[j - 1]; 
	}

	//метод прогонки: обратный ход
	alpha[Num_Segments] /= b[Num_Segments];
	for (int j = Num_Segments - 1; j >= 0; j--)
		alpha[j] = (alpha[j] - alpha[j + 1] * c[j]) / b[j];
}

//вычислить значение сплайна в точке X
double Smoothing_Spline_1d::get_value(const double X, int derivitive) const
{
	double res = 0;
	double eps = 1e-7;

	if (derivitive == 0 || derivitive == 1)
	{
		//поиск отрезка, которому принадлежит точка
		for (int i = 0; i < knots.size() - 1; i++)
		{
			if (X > knots[i] && X < knots[i + 1] ||
				fabs(X - knots[i]) < eps ||
				fabs(X - knots[i + 1]) < eps)
			{
				//длина отрезка
				double h = knots[i + 1] - knots[i];
				//переход на местер-элемент, Ksi - координата на мастер-элементе
				double Ksi = Transition_To_Master_Element(i, X);
				//вычисляем значение сплайна и производной по базисным функциям
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

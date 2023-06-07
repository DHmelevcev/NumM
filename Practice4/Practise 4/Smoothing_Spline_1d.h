#pragma once
#ifndef Smoothing_Spline_1d_h
#define Smoothing_Spline_1d_h

#include "Spline.h"

//сглаживающий сплайн
class Smoothing_Spline_1d : public Spline
{
private:
	//параметр сглаживания
	double SMOOTH;
	//коэффициенты разложения по базису
	std::vector<double> alpha;
	//переход на мастер элемент [-1, 1]: 
	//Seg_Num - номер сегмента, Х - абсцисса, Ksi - координата на мастер-элементе
	double Transition_To_Master_Element(int Seg_Num, const double &X) const;
	//базисные функции на [-1, 1]:
	//Number - номер функции, Ksi - координата на мастер-элементе
	double Basis_Function(int Number, const double &Ksi) const;
	//производные базисных функций на [-1, 1]:
	//Number - номер функции, Ksi - координата на мастер-элементе
	double Der_Basis_Function(int Number, const double & Ksi) const;
public:
	//конструктор: SMOOTH - параметр сглаживания
	Smoothing_Spline_1d(const double &SMOOTH);
	//обновить сплайн
	void update(const std::vector<double>& points, const std::vector<double>& values) override;
	//вычислить значение сплайна в точке P
	double get_value(const double point, int derivitive = 0) const override;
};

#endif
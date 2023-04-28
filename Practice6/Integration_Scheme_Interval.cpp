#include "Integration_Scheme_Interval.h"

namespace Com_Methods
{
	//конструктор: на вход подаётся тип квадратурной формулы
	Integration_Scheme_Interval::Integration_Scheme_Interval(Integration_Scheme_Type Type)
	{
		//заполнение массивов точек и весов интегрирования
		switch (Type)
		{
			//схема метода Гаусс-1
			case Gauss1:
			{
				Weight = {
							2,
						 };
				Points = {
							Point(0, 0, 0),
						 };
				break;
			}

			//схема метода Гаусс-2
			case Gauss2:
			{
				Weight = {	1,
							1,
						 };
				Points = {	Point(-1.0 / sqrt(3), 0, 0),
							Point(1.0 / sqrt(3), 0, 0),
						 };
				break;
			}

			//схема метода Гаусс-3
			case Gauss3:
			{
				Weight = {	5.0 / 9,
							8.0 / 9,
							5.0 / 9,
						 };
				Points = {
							Point(-sqrt(0.6), 0, 0),
							Point(0, 0, 0),
							Point(sqrt(0.6), 0, 0),
						 };
				break;
			}

			//схема метода Гаусс-4
			case Gauss4:
			{
				Weight = {
							0.5 - sqrt(30) / 36,
							0.5 + sqrt(30) / 36,
							0.5 + sqrt(30) / 36,
							0.5 - sqrt(30) / 36,
						 };
				Points = {
							Point(-sqrt(3.0 / 7 + 2.0 * sqrt(1.2) / 7), 0, 0),
							Point(-sqrt(3.0 / 7 - 2.0 * sqrt(1.2) / 7), 0, 0),
							Point(sqrt(3.0 / 7 - 2.0 * sqrt(1.2) / 7), 0, 0),
							Point(sqrt(3.0 / 7 + 2.0 * sqrt(1.2) / 7), 0, 0),
						 };
				break;
			}

			//схема метода Гаусс-5
			case Gauss5:
			{
				Weight = {	(322.0 - 13 * sqrt(70)) / 900,
							(322.0 + 13 * sqrt(70)) / 900,
							128.0 / 225,
							(322.0 + 13 * sqrt(70)) / 900,
							(322.0 - 13 * sqrt(70)) / 900,
						 };
				Points = {
							Point(-1.0 * sqrt(5.0 + 2.0 * sqrt(10.0 / 7)) / 3, 0, 0),
							Point(-1.0 * sqrt(5.0 - 2.0 * sqrt(10.0 / 7)) / 3, 0, 0),
							Point(0, 0, 0),
							Point(1.0 * sqrt(5.0 + 2.0 * sqrt(10.0 / 7)) / 3, 0, 0),
							Point(1.0 * sqrt(5.0 - 2.0 * sqrt(10.0 / 7)) / 3, 0, 0),
						 };
				break;
			}

			//схема метода трапеций
			case Trap:
			{
				Weight = {
							1,
							1,
						 };
				Points = {
							Point(-1, 0, 0),
							Point(1, 0, 0),
						 };
				break;
			}

			//схема метода парабол
			case Parab:
			{
				Weight = {
							1.0 / 3,
							4.0 / 3,
							1.0 / 3,
						 };
				Points = {
							Point(-1, 0, 0),
							Point(0, 0, 0),
							Point(1, 0, 0),
						 };
				break;
			}
		}
	}

	//метод для вычисления определённого интеграла: 
	//Begin и End - начало и конец отрезка 
	//Num_Segments - число сегментов
	//Func - подынтегральная функция
	double Integration_Scheme_Interval:: Calculate_Integral(
								         const Point &Begin,
								         const Point &End,
								         int Number_Segments,
								         const std::function<double(const Point &P)>&Func) const
	{
		//результат (квадратурная сумма)
		double Result = 0.0;
		//начальная точка сегмента
		double X0 = Begin.x();
		//шаг на отрезке
		double h = (End.x() - Begin.x()) / Number_Segments;
		//сумма по всем сегментам разбиения
		for (int i = 0; i < Number_Segments; i++)
		{
			//сумма по узлам интегрирования
			for (int Integ_Point = 0; Integ_Point < Points.size(); ++Integ_Point)
			{
				//переход с мастер-элемента [-1, 1]
				auto P = Point(X0 + (1 + Points[Integ_Point].x()) * h / 2.0, 0, 0);
				Result += Weight[Integ_Point] * Func(P);
			}

			//начальная точка сегмента
			X0 += h;
		}
		//формируем результат с учётом якобиана на отрезке [-1, 1]
		return Result * (h / 2.0);
	}
}

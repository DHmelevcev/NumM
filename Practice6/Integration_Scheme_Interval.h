#pragma once
#include "Integration_Scheme.h"
#include <functional>

namespace Com_Methods
{
	class Integration_Scheme_Interval : protected Integration_Scheme
	{
	public:

		//конструктор: на вход подаётся тип квадратурной формулы
		Integration_Scheme_Interval(Integration_Scheme_Type Type);

		//метод для вычисления определённого интеграла: 
		//Begin и End - начало и конец отрезка 
		//Num_Segments - число сегментов
		//Func - подынтегральная функция
		double Calculate_Integral(const Point &Begin, 
								  const Point &End, 
								  int Number_Segments,
								  const std::function<double(const Point &P)>&Func) const;
	};
}


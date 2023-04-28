#include <iostream>
#include "Integration_Scheme_Interval.h"

int main()
{
	//подынтегральная функция f(x) = 1/(1+2x)
	std::function<double(const Com_Methods::Point &P)> f = 
	[](const Com_Methods::Point &P) { return 1 / (1 + 2 * P.x()); };
	//первообразная F(x) = ln(2x+1)/2
	std::function<double(const Com_Methods::Point &P)> F =
	[](const Com_Methods::Point &P) { return 0.5 * log(2 * P.x() + 1); };

	//квадратурная формула трапеций
	Com_Methods::Integration_Scheme_Interval Quadrature_Formula1(Com_Methods::Integration_Scheme::Trap);
	//квадратурная формула Гаусс-3
	Com_Methods::Integration_Scheme_Interval Quadrature_Formula2(Com_Methods::Integration_Scheme::Gauss3);

	//начало и конец отрезка интегрирования
	auto Begin = Com_Methods::Point(1.1, 0, 0);
	auto End   = Com_Methods::Point(2.3, 0, 0);

	//число сегментов
	int Num_Segments = 12;
	
	//точное значение интеграла (ф. Ньютона-Лейбница)
	double I_True = F(End) - F(Begin);

	for (int i = 0; i < 3; ++i, Num_Segments *=2)
	{
		//численное значение интеграла
		double I1 = Quadrature_Formula1.Calculate_Integral(Begin, End, Num_Segments, f);
		double I2 = Quadrature_Formula2.Calculate_Integral(Begin, End, Num_Segments, f);

		std::cout.precision(20);
		std::cout << std::scientific;
		std::cout << "h = " << (End.x() - Begin.x()) / Num_Segments << "\n";
		std::cout << "I*   = " << I_True << "\n";
		std::cout << "I_t  = " << I1 << "\n";
		std::cout << "I_g3 = " << I2 << "\n";
		std::cout << "|I_t  - I_True| = " << fabs(I1 - I_True) << "\n";
		std::cout << "|I_g3 - I_True| = " << fabs(I2 - I_True) << "\n\n";
	}
}
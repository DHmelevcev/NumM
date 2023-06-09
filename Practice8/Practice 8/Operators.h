#pragma once
#ifndef Complex_Operators_h
#define Complex_Operators_h

#include <complex>
#include <vector>
#include "CONST.h"

namespace Com_Methods
{
	class Operators
	{
	public:
		//оператор сдвига
		//k - величина сдвига, Data - входные данные, Result - массив под результат
		void Cyclic_Shift(int k, const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

		//оператор сгущающей выборки
		//l - показатель степени, Data - прообраз, Result - образ
		void Downsampling_Operator(int l, const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

		//оператор разрежающей выборки
		//l - показатель степени, Data - прообраз, Result - образ
		void Upsampling_Operator(int l, const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

		//скалярное произведение двух векторов
	    std::complex<double> Dot_Product(const std::vector<std::complex<double>>& Vec1, const std::vector<std::complex<double>>& Vec2);
	};
}

#endif // ! Complex_Operators_h


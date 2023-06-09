#pragma once
#ifndef Discrete_Fourier_Transformation_h
#define Discrete_Fourier_Transformation_h

#include <vector>
#include <complex>
#include "CONST.h"

namespace Com_Methods
{
	//преобразование Фурье
	class Discrete_Fourier_Transformation
	{
	public:

		//быстрое преобразование (длина вектора - чётное число)
		//Data - входные данные, Result - массив результата
		void FFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

		//------------------------------------------------------------------------------------------

		//обратное быстрое преобразование (длина вектора чётная)
		//Data - входные данные, Result - массив результата
		void IFFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

		//------------------------------------------------------------------------------------------

		//свёртка комплексных векторов res(m) = Vec1(m - n) * Vec2
		//Vec1 и Vec2 - входные данные, Result - массив результата
		void Convolution(const std::vector<std::complex<double>>& Vec1,
						 const std::vector<std::complex<double>>& Vec2,
						 std::vector<std::complex<double>>& Result);
	};
}
#endif // !Discrete_Fourier_Transformation_h


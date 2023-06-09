#include "Discrete_Fourier_Transformation.h"

namespace Com_Methods
{
	//быстрое преобразование (длина вектора - чётное число)
	//Data - входные данные, Result - массив результата
	void Discrete_Fourier_Transformation::FFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
	{
		int N = Data.size(), M = N / 2;
		Result.clear(); Result.resize(N);
		std::complex<double> Exp, U, V;
		
		for (int m = 0; m < M; m++)
		{
			U._Val[0] = 0.0; U._Val[1] = 0.0;
			V._Val[0] = 0.0; V._Val[1] = 0.0;
			for (int n = 0; n < M; n++)
			{
				Exp._Val[0] = cos(-2.0 * PI * m * n / M);
				Exp._Val[1] = sin(-2.0 * PI * m * n / M);
				U += Data[2 * n] * Exp;
				V += Data[2 * n + 1] * Exp;
			}

			Exp._Val[0] = cos(-2.0 * PI * m / N);
			Exp._Val[1] = sin(-2.0 * PI * m / N);
			Result[m] = U + Exp * V;
			Result[m + M] = U - Exp * V;
		}
	}

	//------------------------------------------------------------------------------------------

	//обратное быстрое преобразование (длина вектора чётная)
	//Data - входные данные, Result - массив результата
	void Discrete_Fourier_Transformation::IFFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
	{
		int N = Data.size();
		FFT(Data, Result);
		std::complex<double> Val;
		for (int i = 1; i <= N / 2; i++)
		{
			Val = Result[i];
			Result[i] = Result[N - i] / double(N);
			Result[N - i] = Val / double(N);
		}

		Result[0] /= double(N);
	}

	//------------------------------------------------------------------------------------------

	//свёртка комплексных векторов res(m) = Vec1(m - n) * Vec2
	//Vec1 и Vec2 - входные данные, Result - массив результата
	void Discrete_Fourier_Transformation::Convolution(const std::vector<std::complex<double>>& Vec1,
													  const std::vector<std::complex<double>>& Vec2,
													  std::vector<std::complex<double>>& Result)
	{
		int N = Vec1.size();
		std::vector<std::complex<double>> Help(N);
		Result.clear(); Result.resize(N);

		FFT(Vec1, Result);
		FFT(Vec2, Help);
		for (int i = 0; i < N; i++) Help[i] *= Result[i];

		IFFT(Help, Result);
	}
}
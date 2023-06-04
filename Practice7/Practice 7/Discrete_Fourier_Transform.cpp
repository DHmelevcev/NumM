#include "Discrete_Fourier_Transform.h"

void DFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
{
	int N = Data.size();
	Result.clear(); Result.resize(N);
	std::complex<double> Exp, U;

	for (int m = 0; m < N; m++)
	{
		U._Val[0] = 0.0; U._Val[1] = 0.0;
		for (int n = 0; n < N; n++)
		{
			Exp._Val[0] = cos(-2.0 * PI * m * n / N);
			Exp._Val[1] = sin(-2.0 * PI * m * n / N);
			U += Data[n] * Exp;
		}
		Result[m] = U;
	}
}

void IDFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
{
	int N = Data.size();
	DFT(Data, Result);
	std::complex<double> Val;
	for (int i = 1; i <= N / 2; i++)
	{
		Val = Result[i];
		Result[i] = Result[N - i] / double(N);
		Result[N - i] = Val / double(N);
	}

	Result[0] /= double(N);
}

void FFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
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

void IFFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result)
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
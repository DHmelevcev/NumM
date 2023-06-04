#pragma once
#include <vector>
#include <complex>
#define PI 3.1415926535897932

//преобразование
//Data - входные данные, Result - массив результата
void DFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//обратное преобразование
//Data - входные данные, Result - массив результата
void IDFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//быстрое преобразование (длина вектора - чётное число)
//Data - входные данные, Result - массив результата
void FFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//обратное быстрое преобразование (длина вектора чётная)
//Data - входные данные, Result - массив результата
void IFFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);
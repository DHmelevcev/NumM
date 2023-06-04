#pragma once
#include <vector>
#include <complex>
#define PI 3.1415926535897932

//��������������
//Data - ������� ������, Result - ������ ����������
void DFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//�������� ��������������
//Data - ������� ������, Result - ������ ����������
void IDFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//������� �������������� (����� ������� - ������ �����)
//Data - ������� ������, Result - ������ ����������
void FFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);

//�������� ������� �������������� (����� ������� ������)
//Data - ������� ������, Result - ������ ����������
void IFFT(const std::vector<std::complex<double>>& Data, std::vector<std::complex<double>>& Result);
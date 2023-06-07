#pragma once
#ifndef Smoothing_Spline_1d_h
#define Smoothing_Spline_1d_h

#include "Spline.h"

//������������ ������
class Smoothing_Spline_1d : public Spline
{
private:
	//�������� �����������
	double SMOOTH;
	//������������ ���������� �� ������
	std::vector<double> alpha;
	//������� �� ������ ������� [-1, 1]: 
	//Seg_Num - ����� ��������, � - ��������, Ksi - ���������� �� ������-��������
	double Transition_To_Master_Element(int Seg_Num, const double &X) const;
	//�������� ������� �� [-1, 1]:
	//Number - ����� �������, Ksi - ���������� �� ������-��������
	double Basis_Function(int Number, const double &Ksi) const;
	//����������� �������� ������� �� [-1, 1]:
	//Number - ����� �������, Ksi - ���������� �� ������-��������
	double Der_Basis_Function(int Number, const double & Ksi) const;
public:
	//�����������: SMOOTH - �������� �����������
	Smoothing_Spline_1d(const double &SMOOTH);
	//�������� ������
	void update(const std::vector<double>& points, const std::vector<double>& values) override;
	//��������� �������� ������� � ����� P
	double get_value(const double point, int derivitive = 0) const override;
};

#endif
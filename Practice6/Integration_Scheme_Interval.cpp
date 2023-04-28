#include "Integration_Scheme_Interval.h"

namespace Com_Methods
{
	//�����������: �� ���� ������� ��� ������������ �������
	Integration_Scheme_Interval::Integration_Scheme_Interval(Integration_Scheme_Type Type)
	{
		//���������� �������� ����� � ����� ��������������
		switch (Type)
		{
			//����� ������ �����-1
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

			//����� ������ �����-2
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

			//����� ������ �����-3
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

			//����� ������ �����-4
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

			//����� ������ �����-5
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

			//����� ������ ��������
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

			//����� ������ �������
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

	//����� ��� ���������� ������������ ���������: 
	//Begin � End - ������ � ����� ������� 
	//Num_Segments - ����� ���������
	//Func - ��������������� �������
	double Integration_Scheme_Interval:: Calculate_Integral(
								         const Point &Begin,
								         const Point &End,
								         int Number_Segments,
								         const std::function<double(const Point &P)>&Func) const
	{
		//��������� (������������ �����)
		double Result = 0.0;
		//��������� ����� ��������
		double X0 = Begin.x();
		//��� �� �������
		double h = (End.x() - Begin.x()) / Number_Segments;
		//����� �� ���� ��������� ���������
		for (int i = 0; i < Number_Segments; i++)
		{
			//����� �� ����� ��������������
			for (int Integ_Point = 0; Integ_Point < Points.size(); ++Integ_Point)
			{
				//������� � ������-�������� [-1, 1]
				auto P = Point(X0 + (1 + Points[Integ_Point].x()) * h / 2.0, 0, 0);
				Result += Weight[Integ_Point] * Func(P);
			}

			//��������� ����� ��������
			X0 += h;
		}
		//��������� ��������� � ������ �������� �� ������� [-1, 1]
		return Result * (h / 2.0);
	}
}

#pragma once

#ifndef Wavelet_Analysis_H
#define Wavelet_Analysis_H

#include <vector>
#include <complex>
#include "Operators.h"
#include "Discrete_Fourier_Transformation.h"

namespace Com_Methods
{
	class Wavelet_Analysis
	{
	private:
		//отцовский и материнский вэйвлеты
		std::vector<std::complex<double>> U, V;
		//векторы f и g, сдвиги которых порождают p-этапные вэйвлет-базисы
		std::vector<std::vector<std::complex<double>>> f, g;

	public:
		//типы базисов
		enum class Basis_Type
		{
			//комплексный базис Шеннона
			Complex_Shannon = 1,
			//базис Добеши
			Daubechies_6,
		};

		//конструктор: заполнение массивов отцовского и материнского вэйвлетов
		//Count_Data - число отсчётов в векторе сигнала, Type - тип вэйвлет-базиса
		Wavelet_Analysis(int Count_Data, Basis_Type Type);

	private:
		//метод для построения системы вэйвлет-фильтров
		//Stages - количество этапов
		void Wavelet_Filters_System(int Stages);
	
		//метод для построения вэйвлет-базиса
		//Stage - номер этапа
		//Psi - высокочастотная компонента
		//Fi  - низкочастотная компонента
		void Wavelet_Basis(int Stage,
						   std::vector<std::vector<std::complex<double>>>& Psi,
						   std::vector<std::vector<std::complex<double>>>& Fi);

	public:
		//фаза анализа сигнала
		//Stage - номер этапа
		//Data  - вектор сигнала
		//Koef_Psi - коэффициенты разложения по высокочастотной компоненте
		//Koef_Fi  - коэффициенты разложения по низкочастотной компоненте
		void Analysis_Phase(int Stage, 
							const std::vector<std::complex<double>>& Data,
							std::vector<std::complex<double>>& Koef_Psi,
							std::vector<std::complex<double>>& Koef_Fi);

		//фаза синтеза (восстановление сигнала)
		//Stage - номер этапа
		//Koef_Psi  - коэффициенты разложения по высокочастотной компоненте
		//Koef_Fi   - коэффициенты разложения по низкочастотной компоненте
		//P и Q - векторы ортогональных проекций сигнала на множества {Psi} и {Fi} этапа Stage
		//Recovery  - восстановленный сигнал на этапе Stage
		void Synthesis_Phase(int Stage,
								const std::vector<std::complex<double>>& Koef_Psi,
								const std::vector<std::complex<double>>& Koef_Fi,
								std::vector<std::complex<double>>& P,
								std::vector<std::complex<double>>& Q,
								std::vector<std::complex<double>>& Recovery);
	};
}

#endif

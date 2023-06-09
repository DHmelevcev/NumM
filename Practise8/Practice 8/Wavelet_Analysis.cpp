#include "Wavelet_Analysis.h"

namespace Com_Methods
{
	//конструктор: заполнение массивов отцовского и материнского вэйвлетов
	//Count_Data - число отсчётов в векторе сигнала, Type - тип вэйвлет-базиса
	Wavelet_Analysis::Wavelet_Analysis(int Count_Data, Basis_Type Type)
	{
		//число отсчётов в векторе сигнала
		int N = Count_Data;
		U.clear(); U.resize(N);
		V.clear(); V.resize(N);

		switch (Type)
		{
			//комплексный базис Шеннона
			case Basis_Type::Complex_Shannon:
			{
				if (N % 4 != 0) throw std::exception("\nError in Complex Shannon Basis: N % 4 != 0 ...\n");
					
				std::complex<double> Val;
					
				U[0] = V[0] = 1.0 / sqrt(2.0);
					
				for (int i = 1; i < N; i++)
				{
					Val._Val[0] =  sqrt(2.0) / N * cos(PI * i / N) * sin(PI * i / 2.0) / sin(PI * i / N);
					Val._Val[1] = -sqrt(2.0) / N * sin(PI * i / N) * sin(PI * i / 2.0) / sin(PI * i / N);
					U[i] = Val;
					V[i] = pow(-1, i) * Val;
				}
				break;
			}

			//базис Добеши
			case Basis_Type::Daubechies_6:
			{
				double a = 1 - sqrt(10), b = 1 + sqrt(10), c = sqrt(5 + 2 * sqrt(10));

				U[0] = (sqrt(2) / 32) * (b + c);
				U[1] = (sqrt(2) / 32) * (2 * a + 3 * b + 3 * c);
				U[2] = (sqrt(2) / 32) * (6 * a + 4 * b + 2 * c);
				U[3] = (sqrt(2) / 32) * (6 * a + 4 * b - 2 * c);
				U[4] = (sqrt(2) / 32) * (2 * a + 3 * b - 3 * c);
				U[5] = (sqrt(2) / 32) * (b - c);

				for (int i = 6; i < N; i++)
					U[i] = 0;
				
				V[N - 4] = -U[5];
				V[N - 3] = U[4];
				V[N - 2] = -U[3];
				V[N - 1] = U[2];
				V[0] = -U[1];
				V[1] = U[0];

				for (int i = 2; i < N - 4; i++)
					V[i] = 0;

				break;
			}
		}
	}
	
	//--------------------------------------------------------------------------------------------------------------------

	//построение системы вэйвлет-фильтров
	//Stages - количество этапов 
	void Wavelet_Analysis::Wavelet_Filters_System(int Stages)
	{
		//операторы выборок
		Operators Operator;

		//число отсчётов в векторе сигнала
		int N = U.size();

		//вэйвлет-фильтры
		std::vector<std::vector<std::complex<double>>> U_Filters(Stages), V_Filters(Stages);

		//фильтры 1-этапа
		U_Filters[0] = U;
		V_Filters[0] = V;

		//остальные фильтры
		for (int i = 1; i < Stages; i++)
		{
			//число элементов в фильтрах i-этапа
			int Count_Elements = N / int(pow(2, i));

			U_Filters[i].resize(Count_Elements);
			V_Filters[i].resize(Count_Elements);

			//заполнение фильтров i-этапа
			for (int n = 0; n < Count_Elements; n++)
			{
				int Max_Index = int(pow(2, i));
				for (int k = 0; k < Max_Index; k++)
				{
					U_Filters[i][n] += U_Filters[0][n + k * N / Max_Index];
					V_Filters[i][n] += V_Filters[0][n + k * N / Max_Index];
				}
			}
		}

		//методы DFT для операции свёртки "*"
		Com_Methods::Discrete_Fourier_Transformation DFT;
		
		//векторы для хранения промежуточного результата разрежения данных
		std::vector<std::complex<double>> U_u, U_v;

		//формируем векторы f и g
		f.resize(Stages); g.resize(Stages);
		f[0] = V_Filters[0];
		g[0] = U_Filters[0];
		for (int i = 1; i < Stages; i++)
		{
			//разрежение фильтра U_i
			Operator.Upsampling_Operator(i, U_Filters[i], U_u);
			//разрежение фильтра V_i
			Operator.Upsampling_Operator(i, V_Filters[i], U_v);
			//свёртка
			DFT.Convolution(g[i - 1], U_v, f[i]);
			DFT.Convolution(g[i - 1], U_u, g[i]);
		}
	}
	
	//--------------------------------------------------------------------------------------------------------------------

	//построение вэйвлет-базиса
	//Stage - номер этапа
	//Psi - высокочастотная компонента
	//Fi  - низкочастотная компонента
	void Wavelet_Analysis::Wavelet_Basis(int Stage,
										 std::vector<std::vector<std::complex<double>>>& Psi,
										 std::vector<std::vector<std::complex<double>>>& Fi)
	{
		//операторы сдвига и скалярного произведения
		Operators Operator;

		//число отсчётов
		int Count_Data = U.size();

		//число элементов в вэйвлет-базисe Stage-этапа 
		int Count_Bas_Elements = Count_Data / int(pow(2, Stage));

		//проверка на существование системы фильтров Stage-этапа
		if (g.size() < Stage) Wavelet_Filters_System(Stage + 1);

		//формируем элементы вэйвлет-базиса
		Psi.resize(Count_Bas_Elements); Fi.resize(Count_Bas_Elements);
			
		for (int i = 0; i < Count_Bas_Elements; i++)
		{
			int Index = int(pow(2, Stage)) * i;
			Operator.Cyclic_Shift(Index, f[Stage - 1], Psi[i]);
			Operator.Cyclic_Shift(Index, g[Stage - 1], Fi[i]);
		}
	}

	//--------------------------------------------------------------------------------------------------------------------

	//фаза анализа сигнала
	//Stage - номер этапа
	//Data  - вектор сигнала
	//Koef_Psi - коэффициенты разложения по высокочастотной компоненте
	//Koef_Fi  - коэффициенты разложения по низкочастотной компоненте
	void  Wavelet_Analysis::Analysis_Phase(int Stage,
										   const std::vector<std::complex<double>>& Data,
										   std::vector<std::complex<double>>& Koef_Psi,
										   std::vector<std::complex<double>>& Koef_Fi)
	{
		//оператор скалярного произведения
		Operators Operator;

		//вэйвлет-базис
		std::vector<std::vector<std::complex<double>>> Psi, Fi;

		//загрузка вэйвлет-базиса
		Wavelet_Basis(Stage, Psi, Fi);

		//количество базисных элементов
		int Count_Bas_Elements = Psi.size();

		//фаза анализа
		for (int Bas_Element_Index = 0; Bas_Element_Index < Count_Bas_Elements; Bas_Element_Index++)
		{
			//фаза анализа: скалярные произведения вектора сигнала на базисные векторы
			Koef_Psi.push_back(Operator.Dot_Product(Data, Psi[Bas_Element_Index]));
			Koef_Fi.push_back (Operator.Dot_Product(Data, Fi [Bas_Element_Index]));
		}
	}

	//--------------------------------------------------------------------------------------------------------------------

	//фаза синтеза (восстановление сигнала)
	//Stage - номер этапа
	//Koef_Psi  - коэффициенты разложения по высокочастотной компоненте
	//Koef_Fi   - коэффициенты разложения по низкочастотной компоненте
	//P и Q - векторы ортогональных проекций сигнала на множества {Psi} и {Fi} этапа Stage
	//Recovery  - восстановленный сигнал на этапе Stage
	void  Wavelet_Analysis::Synthesis_Phase(int Stage,
											const std::vector<std::complex<double>>& Koef_Psi,
											const std::vector<std::complex<double>>& Koef_Fi,
											std::vector<std::complex<double>>& P,
											std::vector<std::complex<double>>& Q,
											std::vector<std::complex<double>>& Recovery)
	{
		//вэйвлет-базис
		std::vector<std::vector<std::complex<double>>> Psi, Fi;

		//загрузка вэйвлет-базиса
		Wavelet_Basis(Stage, Psi, Fi);

		//количество базисных элементов
		int Count_Bas_Elements = Psi.size();

		//количество отсчётов сигнала
		int Count_Data = U.size();

		//фаза синтеза: вычисление частичной сумммы ряда разложения сигнала по базису 
		for (int Data_Index = 0; Data_Index < Count_Data; Data_Index++)
		{
			//переменные для накопления сумм P и Q
			std::complex<double> P_Stage(0, 0), Q_Stage(0, 0);

			for (int Bas_Element_Index = 0; Bas_Element_Index < Count_Bas_Elements; Bas_Element_Index++)
			{
				//частичное восстановление на Stage-этапе
				P_Stage += Koef_Fi[Bas_Element_Index]  * Fi[Bas_Element_Index][Data_Index];
				//дополнительная информация о восстановлении на (Stage + 1)-этапе
				Q_Stage += Koef_Psi[Bas_Element_Index] * Psi[Bas_Element_Index][Data_Index];

			}
			P.push_back(P_Stage);
			Q.push_back(Q_Stage);
			Recovery.push_back(P[Data_Index] + Q[Data_Index]);
		}
	}
}

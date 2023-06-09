#include <iostream>
#include <iomanip>
#include "Wavelet_Analysis.h"
#include "./UI/scene.h"

constexpr int WINX = XCELLS * CELL_W + 40, WINY = YCELLS * CELL_H + 40;
double Vscale = 0.1, Tscale = 0.0125;

int main()
{
	//число отсчётов
	int N = 512;
	//число этапов
	int Stage = 4;

	//сигнал и его коэффициенты разложения по вэйвлет-базису
	std::vector<std::complex<double>> Z(N), Koef_Psi, Koef_Fi;

	//частичное восстановление, уточняющие данные и восстановленный сигнал
	std::vector<std::complex<double>> P, Q, Z_Rec;

	//формирование исходного сигнала
	for (double n = 0; n < N; n++)
	{
		if (n >= 128 && n <= 255) 
			Z[n]._Val[0] = sin(fabs(pow(n - 128, 1.87)) / 128.0);
		if (n >= 384 && n <= 447) 
			Z[n]._Val[0] = sin(fabs(pow(n - 128, 2.31)) / 128.0);
	}

	//выбор базисной системы
	Com_Methods::Wavelet_Analysis Wavelet_Test(N, Com_Methods::Wavelet_Analysis::Basis_Type::Complex_Shannon);

	//фаза анализа данных
	Wavelet_Test.Analysis_Phase(Stage, Z, Koef_Psi, Koef_Fi);

	//фаза восстановления
	Wavelet_Test.Synthesis_Phase(Stage, Koef_Psi, Koef_Fi, P, Q, Z_Rec);

	std::vector<std::complex<double>> Error(N);
	for (double i = 0; i < N; i++)
		Error[i] = Z[i] - Z_Rec[i];

	////печать: исходный и восстановленный сигнал
	//int SETW = 22;
	//std::cout << std::left << std::setw(SETW) << "Number" << std::setw(SETW) << "Z" << std::setw(SETW) << "Z_Recovery" << std::setw(SETW) << "Error" << std::endl;
	//for (int i = 0; i < Z.size(); i++)
	//{
	//	std::cout << std::left << std::setw(SETW) << i
	//						   << std::setw(SETW) << Z[i].real() 
	//						   << std::setw(SETW) << Z_Rec[i].real()
	//						   << std::setw(SETW) << Error[i].real() << std::endl;
	//}

	//печать: вэйвлет-коэффициенты
	//std::cout << std::endl << std::left << std::setw(SETW) << "Number" << std::setw(SETW) << "|Psi|" << std::setw(SETW) << "|Fi|" << std::endl;
	//for (int i = 0; i < Koef_Fi.size(); i++)
	//{
	//	std::cout << std::left << std::setw(SETW) << pow(2, Stage) * i 
	//					       << std::setw(SETW) << sqrt(pow(Koef_Psi[i].real(),2) + pow(Koef_Psi[i].imag(), 2)) 
	//						   << std::setw(SETW) << sqrt(pow(Koef_Fi[i].real(), 2) + pow(Koef_Fi[i].imag(), 2)) << std::endl;
	//}

	//создание окна отрисовки
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WINX, WINY), "Signal analysis", sf::Style::Default, settings);

	//выбор отображаемого графика
	auto data = Z_Rec;

	MyScene scene;
	size_t plot_size = data.size();
	oscillation plot(plot_size, 1);
	for (int i = 0; i < plot_size; i++)
	{
		plot.X[i] = static_cast<double>(i) / plot_size;
		plot.Y[i] = -data[i].real();
	}

	//отображение графика
	scene.set_plot(plot, Vscale, Tscale);

	while (window.isOpen())
	{
		window.clear(sf::Color(0xf0, 0xf0, 0xf0));

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}

				case sf::Event::MouseWheelScrolled:
				{
					double scroll_speed = 0.01;
					Vscale += Vscale - scroll_speed * event.mouseWheelScroll.delta > 0.01 ? -scroll_speed * event.mouseWheelScroll.delta : -Vscale + 0.01;
					scene.set_plot(plot, Vscale, Tscale);
					break;
				}

				case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						break;
					}

					break;
				}
			}
		}

		scene.draw(window);
		window.display();
	}

	return 0;
}
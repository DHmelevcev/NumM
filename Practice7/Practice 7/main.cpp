#include <iostream>
#include <iomanip>
#include <chrono>
#include "Discrete_Fourier_Transform.h"
#include "Cubic_Interpolation_Spline_1d.h"
#include "./UI/scene.h"

constexpr int WINX = XCELLS * CELL_W + 40, WINY = YCELLS * CELL_H + 40;
double Vscale = 0.2, Tscale = 0.0125;

int main()
{
	//мерность векторного пространства
	size_t N = 512;
	double A = -3;
	double B = 0.59;
	double w = 118;
	double phi0 = PI / 2;

	//Z - данные, DFT_Data - прямое DFT, IDFT_Data - обратное DFT
	std::vector<std::complex<double>> Z(N), DFT_Data(N), IDFT_Data(N);

	//заполнение данных
	for (size_t t = 0; t < N; t++)
	{
		//Z[t]._Val[0] = A + B * cos(2 * PI * t * w / N + phi0);
		Z[t]._Val[0] = cos(2 * PI * t / N) + 0.01 * cos(2 * PI * w * t / N);
		Z[t]._Val[1] = 0;
	}

	int number_of_tests = 1;
	double time1, time2, time3, time4;

	//прямое преобразование
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number_of_tests; i++)
		DFT(Z, DFT_Data);
	auto t2 = std::chrono::high_resolution_clock::now();
	time1 = std::chrono::duration<double>(t2 - t1).count();

	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number_of_tests; i++)
		FFT(Z, DFT_Data);
	t2 = std::chrono::high_resolution_clock::now();
	time2 = std::chrono::duration<double>(t2 - t1).count();

	//отфильтруем сигнал
	DFT_Data[w]._Val[0] = 0;
	DFT_Data[w]._Val[1] = 0;
	DFT_Data[N - w]._Val[0] = 0;
	DFT_Data[N - w]._Val[1] = 0;

	//обратное преобразование
	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number_of_tests; i++)
		IDFT(DFT_Data, IDFT_Data);
	t2 = std::chrono::high_resolution_clock::now();
	time3 = std::chrono::duration<double>(t2 - t1).count();

	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < number_of_tests; i++)
		IFFT(DFT_Data, IDFT_Data);
	t2 = std::chrono::high_resolution_clock::now();
	time4 = std::chrono::duration<double>(t2 - t1).count();

	time1 /= number_of_tests;
	time2 /= number_of_tests;
	time3 /= number_of_tests;
	time4 /= number_of_tests;

	//вывод результата
	const int SETW = 20;
	std::cout	/*<< std::left << std::setw(SETW) << time1
				<< std::left << std::setw(SETW) << time2
				<< std::left << std::setw(SETW) << time3
				<< std::left << std::setw(SETW) << time4 << '\n'*/
				<< std::left << std::setw(SETW) << "m"
				<< std::left << std::setw(SETW) << "Re(Z)"
				<< std::left << std::setw(SETW) << "Re(Z`)"
				<< std::left << std::setw(SETW) << "Re(Z^)"
				<< std::left << std::setw(SETW) << "Im(Z^)"
				<< std::left << std::setw(SETW) << "|Z^|"
				<< std::left << std::setw(SETW) << "phi" << '\n';

	for (size_t m = 0; m < N; m++)
	{
		double x = DFT_Data[m].real();
		double y = DFT_Data[m].imag();
		double eps = 1e-7;
		int sign_y = static_cast<double>(y > eps) - (y < -eps);
		double phi = abs(x) < eps ? sign_y * PI / 2 : x > eps ? atan(y / x) : y >= -eps ? atan(y / x) + PI : atan(y / x) - PI;
		if (pow(pow(x, 2) + pow(y, 2), 0.5) > eps || (abs(phi) - abs(phi0)) > eps)
		{
			std::cout	<< std::left << std::setw(SETW) << m
						<< std::left << std::setw(SETW) << Z[m].real()
						<< std::left << std::setw(SETW) << IDFT_Data[m].real()
						<< std::left << std::setw(SETW) << x
						<< std::left << std::setw(SETW) << y
						<< std::left << std::setw(SETW) << pow(pow(x, 2) + pow(y, 2), 0.5)
						<< std::left << std::setw(SETW) << phi << '\n';
		}
	}

	//создание окна отрисовки
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WINX, WINY), "Signal analysis", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	std::cout	<< "\nControls:\n"
				<< "1: Re(Z)\n"
				<< "2: Re(Z^)\n"
				<< "3: Im(Z^)\n"
				<< "4: |Z^|\n"
				<< "5: phi\n"
				<< "6: Re(Z`)\n"
				<< "s: togle spline smoothing\n"
				<< "mouse wheel: scale\n"
				<< "ESC: exit\n";

	MyScene scene;
	int graph_on_screen = 1;
	bool spline_interpolation = true;

	//построение графика
	Cubic_Interpolation_Spline_1d spline;
	oscillation plot(N, 1);
	for (size_t i = 0; i < N; i++)
	{
		plot.X[i] = static_cast<double>(i) / N;
		plot.Y[i] = -Z[i].real();
	}

	if (spline_interpolation)
	{
		spline.update(plot.X, plot.Y);

		plot = oscillation(int(WINX * 1.1 * static_cast<double>(N - 1) / N), 1);
		for (size_t i = 0; i < plot.X.size(); ++i)
		{
			plot.X[i] = static_cast<double>(i) / (WINX * 1.1);
			plot.Y[i] = spline.get_value(plot.X[i], 0);
		}
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
					double scroll_speed = graph_on_screen == 2 || graph_on_screen == 3 || graph_on_screen == 4 ? 0.5 : 0.05;
					Vscale += Vscale - scroll_speed * event.mouseWheelScroll.delta > 0.05 ? -scroll_speed * event.mouseWheelScroll.delta : -Vscale + 0.05;
					scene.set_plot(plot, Vscale, Tscale);
					//std::cout << "Units per square: " << Vscale << '\n';
					break;
				}

				case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						break;
					}

					if (event.key.code == sf::Keyboard::S ||
						event.key.code == sf::Keyboard::Num1 ||
						event.key.code == sf::Keyboard::Num2 ||
						event.key.code == sf::Keyboard::Num3 ||
						event.key.code == sf::Keyboard::Num4 ||
						event.key.code == sf::Keyboard::Num5 ||
						event.key.code == sf::Keyboard::Num6
						)
					{
						if (event.key.code == sf::Keyboard::S)
						{
							spline_interpolation = !spline_interpolation;
						}

						if (event.key.code == sf::Keyboard::Num1)
						{
							Vscale = graph_on_screen == 2 || graph_on_screen == 3 || graph_on_screen == 4 ? Vscale / 128 : Vscale;
							graph_on_screen = 1;
						}

						if (event.key.code == sf::Keyboard::Num2)
						{
							Vscale = graph_on_screen != 2 && graph_on_screen != 3 && graph_on_screen != 4 ? Vscale * 128 : Vscale;
							graph_on_screen = 2;
						}

						if (event.key.code == sf::Keyboard::Num3)
						{
							Vscale = graph_on_screen != 2 && graph_on_screen != 3 && graph_on_screen != 4 ? Vscale * 128 : Vscale;
							graph_on_screen = 3;
						}

						if (event.key.code == sf::Keyboard::Num4)
						{
							Vscale = graph_on_screen != 2 && graph_on_screen != 3 && graph_on_screen != 4 ? Vscale * 128 : Vscale;
							graph_on_screen = 4;
						}

						if (event.key.code == sf::Keyboard::Num5)
						{
							Vscale = graph_on_screen == 2 || graph_on_screen == 3 || graph_on_screen == 4 ? Vscale / 128 : Vscale;
							graph_on_screen = 5;
						}

						if (event.key.code == sf::Keyboard::Num6)
						{
							Vscale = graph_on_screen == 2 || graph_on_screen == 3 || graph_on_screen == 4 ? Vscale / 128 : Vscale;
							graph_on_screen = 6;
						}

						if (graph_on_screen == 1)
						{
							plot = oscillation(N, 1);
							for (size_t i = 0; i < N; i++)
							{
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -Z[i].real();
							}

							if (spline_interpolation)
							{
								spline.update(plot.X, plot.Y);

								plot = oscillation(int(WINX * 1.1 * static_cast<double>(N - 1) / N), 1);
								for (size_t i = 0; i < plot.X.size(); ++i)
								{
									plot.X[i] = static_cast<double>(i) / (WINX * 1.1);
									plot.Y[i] = spline.get_value(plot.X[i], 0);
								}
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}

						if (graph_on_screen == 2)
						{
							plot = oscillation(N, 1);

							for (size_t i = 0; i < N; ++i)
							{
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -DFT_Data[i].real();
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}
						
						if (graph_on_screen == 3)
						{
							plot = oscillation(N, 1);

							for (size_t i = 0; i < N; ++i)
							{
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -DFT_Data[i].imag();
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}
						
						if (graph_on_screen == 4)
						{
							plot = oscillation(N, 1);

							for (size_t i = 0; i < N; ++i)
							{
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -pow(pow(DFT_Data[i].real(), 2) + pow(DFT_Data[i].imag(), 2), 0.5);
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}

						if (graph_on_screen == 5)
						{
							plot = oscillation(N, 1);

							for (size_t i = 0; i < N; ++i)
							{
								double x = DFT_Data[i].real();
								double y = DFT_Data[i].imag();
								double eps = 1e-7;
								int sign_y = static_cast<double>(y > eps) - (y < -eps);
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -(abs(x) < eps ? sign_y * PI / 2 : x > eps ? atan(y / x) : y >= -eps ? atan(y / x) + PI : atan(y / x) - PI);
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}

						if (graph_on_screen == 6)
						{
							plot = oscillation(N, 1);
							for (size_t i = 0; i < N; i++)
							{
								plot.X[i] = static_cast<double>(i) / N;
								plot.Y[i] = -IDFT_Data[i].real();
							}

							if (spline_interpolation)
							{
								spline.update(plot.X, plot.Y);

								plot = oscillation(int(WINX * 1.1 * static_cast<double>(N - 1) / N), 1);
								for (size_t i = 0; i < plot.X.size(); ++i)
								{
									plot.X[i] = static_cast<double>(i) / (WINX * 1.1);
									plot.Y[i] = spline.get_value(plot.X[i], 0);
								}
							}

							scene.set_plot(plot, Vscale, Tscale);
							break;
						}
					}

					break;
				}
			}
		}

		scene.draw(window);
		window.display();
	}

	return 0;
};
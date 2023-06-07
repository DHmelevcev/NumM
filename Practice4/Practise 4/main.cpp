#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "Cubic_Interpolation_Spline_1d.h"
#include "Smoothing_Spline_1d.h"
#include "./UI/scene.h"

constexpr int WINX = XCELLS * CELL_W + 40, WINY = YCELLS * CELL_H + 40;
double Vscale = 1, Tscale = 0.0125;

int main()
{
	//мерность векторного пространства
	size_t N = 1087;
	//size_t N = 100;

	//Z - данные, DFT_Data - прямое DFT, IDFT_Data - обратное DFT
	std::vector<double> points(N), values(N);

	//заполнение данных
	std::ifstream normdistrbn("normdistrbn.txt");
	for (size_t t = 0; t < N; t++)
	{
		points[t] = static_cast<double>(t) / (N - 1);
		normdistrbn >> values[t] ;
	}
	normdistrbn.close();
	
	//создание окна отрисовки
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WINX, WINY), "Function aproximation", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	MyScene scene;

	Cubic_Interpolation_Spline_1d ci_spline;
	Smoothing_Spline_1d s_spline_0(0);
	Smoothing_Spline_1d s_spline_10(0.1);
	Smoothing_Spline_1d s_spline_70(0.7);
	Smoothing_Spline_1d s_spline_999(0.999);

	ci_spline.update(points, values);
	s_spline_0.update(points, values);
	s_spline_10.update(points, values);
	s_spline_70.update(points, values);
	s_spline_999.update(points, values);

	oscillation plot1(int(WINX * 1.1 * static_cast<double>(N + 1) / N), 1);
	oscillation plot2(int(WINX * 1.1 * static_cast<double>(N + 1) / N), 1);
	oscillation plot3(int(WINX * 1.1 * static_cast<double>(N + 1) / N), 1);
	oscillation plot4(int(WINX * 1.1 * static_cast<double>(N + 1) / N), 1);
	oscillation plot5(int(WINX * 1.1 * static_cast<double>(N + 1) / N), 1);
	oscillation plot0(N, 1);
	oscillation plotempty(0, 1);

	for (size_t i = 0; i < N; i++)
	{
		plot0.X[i] = static_cast<double>(i) / (N - 1);
		plot0.Y[i] = -values[i];
	}

	for (size_t i = 0; i < plot1.X.size(); i++)
	{
		plot1.X[i] = static_cast<double>(i) / (WINX * 1.1) * static_cast<double>(N) / (N + 1);
		plot2.X[i] = static_cast<double>(i) / (WINX * 1.1) * static_cast<double>(N) / (N + 1);
		plot3.X[i] = static_cast<double>(i) / (WINX * 1.1) * static_cast<double>(N) / (N + 1);
		plot4.X[i] = static_cast<double>(i) / (WINX * 1.1) * static_cast<double>(N) / (N + 1);
		plot5.X[i] = static_cast<double>(i) / (WINX * 1.1) * static_cast<double>(N) / (N + 1);
		plot1.Y[i] = -ci_spline.get_value(plot1.X[i]);
		plot2.Y[i] = -s_spline_0.get_value(plot2.X[i]);
		plot3.Y[i] = -s_spline_10.get_value(plot3.X[i]);
		plot4.Y[i] = -s_spline_70.get_value(plot4.X[i]);
		plot5.Y[i] = -s_spline_999.get_value(plot5.X[i]);
	}

	std::vector<bool> graph_on_screen(4, 0);
	scene.set_plot2(plot0, Vscale, Tscale);

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

				case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						break;
					}

					if (event.key.code == sf::Keyboard::Num1)
					{
						graph_on_screen[1] = !graph_on_screen[1];
						graph_on_screen[2] = 0;
						graph_on_screen[3] = 0;
						graph_on_screen[4] = 0;
						graph_on_screen[5] = 0;
					}

					if (event.key.code == sf::Keyboard::Num2)
					{
						graph_on_screen[1] = 0;
						graph_on_screen[2] = !graph_on_screen[2];
						graph_on_screen[3] = 0;
						graph_on_screen[4] = 0;
						graph_on_screen[5] = 0;
					}

					if (event.key.code == sf::Keyboard::Num3)
					{
						graph_on_screen[1] = 0;
						graph_on_screen[2] = 0;
						graph_on_screen[3] = !graph_on_screen[3];
						graph_on_screen[4] = 0;
						graph_on_screen[5] = 0;
					}

					if (event.key.code == sf::Keyboard::Num4)
					{
						graph_on_screen[1] = 0;
						graph_on_screen[2] = 0;
						graph_on_screen[3] = 0;
						graph_on_screen[4] = !graph_on_screen[4];
						graph_on_screen[5] = 0;
					}

					if (event.key.code == sf::Keyboard::Num5)
					{
						graph_on_screen[1] = 0;
						graph_on_screen[2] = 0;
						graph_on_screen[3] = 0;
						graph_on_screen[4] = 0;
						graph_on_screen[5] = !graph_on_screen[5];
					}

					if (event.key.code == sf::Keyboard::Num1 ||
						event.key.code == sf::Keyboard::Num2 ||
						event.key.code == sf::Keyboard::Num3 ||
						event.key.code == sf::Keyboard::Num4 ||
						event.key.code == sf::Keyboard::Num5)
					{
						scene.set_plot1(graph_on_screen[1] ? plot1 :
										graph_on_screen[2] ? plot2 :
										graph_on_screen[3] ? plot3 :
										graph_on_screen[4] ? plot4 :
										graph_on_screen[5] ? plot5 :
										plotempty, Vscale, Tscale);
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
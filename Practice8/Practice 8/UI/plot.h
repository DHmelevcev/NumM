#pragma once
#include "entity.h"
#include "oscillation.h"

const uint16_t XCELLS = 80, YCELLS = 50;
const uint16_t CELL_W = 20, CELL_H = 20;

class MyPlot : public MyEntity
{
public:

	MyPlot(sf::Color color) : m_color(color)
	{
		m_transform.translate(20, 20 + (YCELLS * CELL_H) / 2);

		m_knots.setPrimitiveType(sf::LineStrip);
	}

	bool set(oscillation oscillation, double Vscale, double Tscale)
	{
		m_knots.clear();
		m_knots.resize(oscillation.knots);

		for (int i = 0; i < oscillation.knots; ++i) {
			m_knots[i] = sf::Vertex(sf::Vector2f(static_cast<float>(oscillation.X[i] * oscillation.duration * CELL_W / Tscale), static_cast<float>(oscillation.Y[i] * CELL_H / Vscale)), m_color);
		}

		return true;
	}

private:

	virtual void onDraw(sf::RenderTarget& target) const
	{
		target.draw(m_knots, m_transform);
	}

	sf::Transform m_transform;
	sf::VertexArray m_knots;
	sf::Color m_color;
};
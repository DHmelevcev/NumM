#pragma once
#include "entity.h"
#include "oscillation.h"

class MyGrid : public MyEntity
{
public:

	MyGrid(sf::Color color)
	{
		m_transform.translate(20, 20);

		m_vertices.setPrimitiveType(sf::LineStrip);
		m_vertices.append(sf::Vertex(sf::Vector2f(0, 0), color));
		m_vertices.append(sf::Vertex(sf::Vector2f(XCELLS * CELL_W, 0), color));
		m_vertices.append(sf::Vertex(sf::Vector2f(XCELLS * CELL_W, YCELLS * CELL_H), color));
		m_vertices.append(sf::Vertex(sf::Vector2f(0, YCELLS * CELL_H), color));
		m_vertices.append(sf::Vertex(sf::Vector2f(0, 0), color));
		m_lines.setPrimitiveType(sf::Lines);
		for (int i = 0; i < YCELLS - 1;)
		{
			m_lines.append(sf::Vertex(sf::Vector2f(0, static_cast<float>(++i * CELL_H)), color));
			m_lines.append(sf::Vertex(sf::Vector2f(XCELLS * CELL_W, static_cast<float>(i * CELL_H)), color));
		}
		for (int i = 0; i < XCELLS - 1;)
		{
			m_lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(++i * CELL_W), 0), color));
			m_lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(i * CELL_W), YCELLS * CELL_H), color));
		}
		m_lines.append(sf::Vertex(sf::Vector2f(0, YCELLS * CELL_H / 2), color + sf::Color(0, 0, 0, 20)));
		m_lines.append(sf::Vertex(sf::Vector2f(XCELLS * CELL_W, YCELLS * CELL_H / 2), color + sf::Color(0, 0, 0, 20)));
	}

private:

	virtual void onDraw(sf::RenderTarget& target) const
	{
		target.draw(m_vertices, m_transform);
		target.draw(m_lines, m_transform);
	}
	
    sf::Transform m_transform;
	sf::VertexArray m_vertices;
	sf::VertexArray m_lines;
};
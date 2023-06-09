#pragma once
#include "plot.h"
#include "grid.h"

class MyScene : public MyEntity
{
public:

    MyScene()
    {
        grid = std::make_unique<MyGrid>(sf::Color(0x0, 0x0, 0x0, 50));
        plot = std::make_unique<MyPlot>(sf::Color(0x0, 0x0, 0x0));
        m_children.push_back(&*grid);
        m_children.push_back(&*plot);
    }

    bool set_plot(oscillation oscillation, double Vscale, double Tscale)
    {
        return plot->set(oscillation, Vscale, Tscale);
    }

    void draw(sf::RenderTarget& target) const
    {
        for (std::size_t i = 0; i < m_children.size(); ++i)
            m_children[i]->draw(target);
    }

private:

    virtual void onDraw(sf::RenderTarget& target) const {};

    std::unique_ptr<MyGrid> grid;
    std::unique_ptr<MyPlot> plot;

    sf::Transform m_transform;
    std::vector<MyEntity*> m_children;
};
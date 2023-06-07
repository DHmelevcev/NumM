#pragma once
#include "plot.h"
#include "grid.h"

class MyScene : public MyEntity
{
public:

    MyScene()
    {
        grid = std::make_unique<MyGrid>(sf::Color(0x0, 0x0, 0x0, 50));
        plot1 = std::make_unique<MyPlot>(sf::Color(0x0, 0x0, 0x0));
        plot2 = std::make_unique<MyPlot>(sf::Color(0xFF, 0x0, 0x0, 60));
        m_children.push_back(&*grid);
        m_children.push_back(&*plot1);
        m_children.push_back(&*plot2);
    }

    bool set_plot1(oscillation oscillation, double Vscale, double Tscale)
    {
        return plot1->set(oscillation, Vscale, Tscale);
    }
    
    bool set_plot2(oscillation oscillation, double Vscale, double Tscale)
    {
        return plot2->set(oscillation, Vscale, Tscale);
    }

    void draw(sf::RenderTarget& target) const
    {
        for (std::size_t i = 0; i < m_children.size(); ++i)
            m_children[i]->draw(target);
    }

private:

    virtual void onDraw(sf::RenderTarget& target) const {};

    std::unique_ptr<MyGrid> grid;
    std::unique_ptr<MyPlot> plot1;
    std::unique_ptr<MyPlot> plot2;

    sf::Transform m_transform;
    std::vector<MyEntity*> m_children;
};
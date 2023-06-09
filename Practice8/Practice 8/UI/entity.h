#pragma once
#include <SFML/Graphics.hpp>

class MyEntity
{
public:

    virtual void draw(sf::RenderTarget& target) const
    {
        onDraw(target);

        for (std::size_t i = 0; i < m_children.size(); ++i)
            m_children[i]->draw(target);
    }

private:

    virtual void onDraw(sf::RenderTarget& target) const = 0;

    sf::Transform m_transform;
    std::vector<MyEntity*> m_children;
};
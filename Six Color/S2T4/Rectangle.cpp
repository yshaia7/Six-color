#include "Rectangle.h"



Rectangle::Rectangle()
{
	m_shape.setPointCount(4);
	m_shape.setPoint(0, sf::Vector2f(0, 0));
	m_shape.setPoint(1, sf::Vector2f(M_SIZE / 2, M_SIZE / 2));
	m_shape.setPoint(2, sf::Vector2f(0, M_SIZE));
	m_shape.setPoint(3, sf::Vector2f(-(M_SIZE / 2), M_SIZE / 2));
	m_shape.setOrigin(sf::Vector2f(m_shape.getLocalBounds().left + m_shape.getLocalBounds().width / 2, m_shape.getLocalBounds().top + m_shape.getLocalBounds().height));
}
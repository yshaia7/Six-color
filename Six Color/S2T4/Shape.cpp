#include "Shape.h"



Shape::Shape()
{
	m_shape.setOutlineColor(sf::Color::Black);
	m_shape.setOutlineThickness(-0.2);
	m_ownership = 0;
	setVisitedState(false);
	randomizeColor(0);
}

void Shape::randomizeColor(int owner)
{
	std::string colors = { "ryogbp" };
	int randomColor = rand() % 6;
	setColor(m_shape, owner, colors[randomColor]);
}

void Shape::setColor(sf::ConvexShape& shape,int owner, char newColor)
{
	if (m_ownership == owner)
	{
		switch (newColor)
		{
		case 'r':
		{
			shape.setFillColor(sf::Color(255, 0, 0));
			break;
		}
		case 'y':
		{
			shape.setFillColor(sf::Color(255, 244, 78));
			break;
		}
		case 'o':
		{
			shape.setFillColor(sf::Color(255, 106, 0));
			break;
		}
		case 'g':
		{
			shape.setFillColor(sf::Color(78, 255, 0));
			break;
		}
		case 'b':
		{
			shape.setFillColor(sf::Color(0, 38, 255));
			break;
		}
		case 'p':
		{
			shape.setFillColor(sf::Color(255, 0, 220));
			break;
		}
		}
		m_color = newColor;
	}

}

void Shape::setColor(int owner, char newColor)
{
	if (m_ownership == owner)
		setColor(m_shape, owner, newColor);
}

void Shape::setLocation(int x, int y)
{
	if (y % 2 == 0)
		m_shape.setPosition(sf::Vector2f(M_START + (M_SIZE * x), M_START + (M_SIZE * y) / 2));
	else
		m_shape.setPosition(sf::Vector2f(M_START + (M_SIZE * x) + (M_SIZE / 2), M_START + (M_SIZE * y) / 2));
}

void Shape::markNewOwner(char color, int owner)
{
	if (m_ownership == owner)
		for (auto i : m_neighbors)
			if (i != nullptr)
				markNewOwnerRecursion(i, color, owner);
}

void Shape::markNewOwnerRecursion(std::shared_ptr<Shape> shape, char color, int owner)
{
	if (shape->getColor() != color || shape->getOwnership() != 0)
		return;
	shape->setOwnership(owner);
	for (auto i : shape->getNeighbors())
		if (i != nullptr)
			markNewOwnerRecursion(i, color, owner);
}

void Shape::countColors(std::vector<int>& colorVector) //work in progress
{
	if (m_ownership != 2)
		return;
	for (auto i : m_neighbors)
		if (i != nullptr)
			colorCounterRecursion(i, i->getColor(), colorVector);
}

void Shape::colorCounterRecursion(std::shared_ptr<Shape> shape, char color, std::vector<int>& colorVector)
{
	if (shape->getOwnership() != 0 || shape->getColor() != color || shape->getVisitedState())
		return;
	shape->setVisitedState(true);
	switch (color)
	{
		case 'r':
			colorVector[0]++;
			break;
		case 'o':
			colorVector[1]++;
			break;
		case 'y':
			colorVector[2]++;
			break;
		case 'g':
			colorVector[3]++;
			break;
		case 'b':
			colorVector[4]++;
			break;
		case 'p':
			colorVector[5]++;
			break;
	}
	for (auto i : shape->getNeighbors())
		if (i != nullptr)
			colorCounterRecursion(i, color, colorVector);
}

void Shape::setTexture(sf::Texture& playerTexture)
{
	if (m_ownership == 1)
		m_shape.setTexture(&playerTexture);
}

void Shape::removeTexture()
{
	if (m_ownership == 1)
		m_shape.setTexture(NULL);
}
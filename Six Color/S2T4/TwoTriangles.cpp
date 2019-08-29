#include "TwoTriangles.h"



TwoTriangles::TwoTriangles()
{
	triOne = std::make_shared<Triangle>();
	triTwo = std::make_shared<Triangle>();
	int randNum;
	triTwo->rotate(180);
	randNum = rand() % 2;
	if (randNum == 0) //Select randomly if the triangles are split horizontally or vertically
	{
		triOne->rotate(90);
		triTwo->rotate(90);
		m_direction = 'v';
	}
	else
		m_direction = 'h';
}

void TwoTriangles::setLocation(int x, int y)
{
	triOne->setLocation(x, y);
	triOne->moveShape(0, -M_SIZE / 2);
	triTwo->setLocation(x, y);
	triTwo->moveShape(0, -M_SIZE / 2);
}

void TwoTriangles::draw(sf::RenderWindow& window)
{
	triOne->draw(window);
	triTwo->draw(window);
}


void TwoTriangles::addNeighbor(std::shared_ptr<Shape> newNeighbor, int direction)
{
	Shape::addNeighbor(newNeighbor, direction);
	if (m_neighbors.size() == 4)
	{
		triOne->addNeighbor(m_neighbors[1], 0);
		triTwo->addNeighbor(m_neighbors[3], 0);
		if (m_direction == 'h')
		{
			triOne->addNeighbor(m_neighbors[0], 0);
			triOne->addNeighbor(triTwo, 0);
			triTwo->addNeighbor(m_neighbors[2], 0);
			triTwo->addNeighbor(triOne, 0);
		}
		else
		{
			triOne->addNeighbor(m_neighbors[2], 0);
			triOne->addNeighbor(triTwo, 0);
			triTwo->addNeighbor(m_neighbors[0], 0);
			triTwo->addNeighbor(triOne, 0);
		}
	}
}

std::shared_ptr<Shape> TwoTriangles::getShape(int direction)
{
	if ((m_direction == 'v' && (direction == 0 || direction == 2)) || (m_direction == 'h' && (direction == 2 || direction == 3)))
		return triOne;
	return triTwo;
}

void TwoTriangles::markNewOwner(char color, int owner)
{
	if (triOne->getOwnership() == owner)
		triOne->markNewOwner(color, owner);
	if (triTwo->getOwnership() == owner)
		triTwo->markNewOwner(color, owner);
	if (triOne->getOwnership() == triTwo->getOwnership())
		m_ownership = triOne->getOwnership();
	if ((triOne->getOwnership() == 1 && triTwo->getOwnership() == 2) || (triOne->getOwnership() == 2 && triTwo->getOwnership() == 1))
		m_ownership = 3; //Each player owns half of the shape
}

void TwoTriangles::setColor(int owner, char newColor)
{
	triOne->setColor(owner, newColor);
	triTwo->setColor(owner, newColor);
}

void TwoTriangles::countColors(std::vector<int>& counter)
{
	triOne->countColors(counter);
	triTwo->countColors(counter);
}

void TwoTriangles::setTexture(sf::Texture& playerTexture)
{
	triOne->setTexture(playerTexture);
	triTwo->setTexture(playerTexture);
}

void TwoTriangles::removeTexture()
{
	triOne->removeTexture();
	triTwo->removeTexture();
}
#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle();
	~Triangle() {}
	void moveShape(float x, float y) { m_shape.move(sf::Vector2f(x, y)); } //Move triangle to desired location
};


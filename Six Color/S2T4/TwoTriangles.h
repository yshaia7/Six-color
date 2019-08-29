#pragma once
#include "Triangle.h"
#include <SFML/Graphics/CircleShape.hpp>

class TwoTriangles : public Triangle
{
public:
	TwoTriangles();
	~TwoTriangles() {}
	//All override functions use the appropriate Shape function on both triangles
	void setLocation(int x, int y) override;
	void draw(sf::RenderWindow& window) override;
	void addNeighbor(std::shared_ptr<Shape> newNeighbor, int direction) override;
	void addNullNeighbor() override { addNeighbor(nullptr, 0); } 
	void markNewOwner(char color, int owner) override;
	void setColor(int owner, char newColor) override;
	void countColors(std::vector<int>& counter) override;
	void setTexture(sf::Texture& playerTexture) override;
	void removeTexture() override;
	int getShapeNum() override { return 2; }
	

private:
	char m_direction; //Determines how the triangles are split. v = vertical split, h = horizontal split
	std::shared_ptr<Triangle> triOne;
	std::shared_ptr<Triangle> triTwo;
	std::shared_ptr<Shape> getShape(int direction); //Returns triangle based on the direction of the shape that requested it 
};


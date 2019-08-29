#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

const int M_SIZE = 30; //Shape size
const int M_START = 60; //Shape drawing starting location

class Shape : public std::enable_shared_from_this<Shape>
{
public:
	Shape();
	~Shape() {}
	virtual void addNeighbor(std::shared_ptr<Shape> newNeighbor, int direction) { m_neighbors.push_back(newNeighbor->getShape(direction)); } //Add another neighbor to the the shape
	virtual void addNullNeighbor() { m_neighbors.push_back(nullptr); } //Add null pointer to the neighbor list
	virtual void setLocation(int x, int y); //Change location of shape
	virtual void draw(sf::RenderWindow& window) { window.draw(m_shape); } //Draw shape
	virtual void setColor(int owner, char newColor); //Change color of shape
	sf::Vector2f getPosition() { return m_shape.getPosition(); }
	void setVisitedState(bool newState) { m_visited = newState; } //Mark whether a shape has been visited or not
	bool getVisitedState() { return m_visited; } //Check whether a shape has been visited or not
	char getColor() { return m_color; } //Get shape color
	void setOwnership(int owner) { m_ownership = owner; } //Set shape owner
	int getOwnership() { return m_ownership; } //Get shape owner
	std::vector<std::shared_ptr<Shape>>& getNeighbors() { return m_neighbors; } //Get neighbor list
	int getSize() { return M_SIZE; } //Get shape size
	void rotate(float degree) { m_shape.rotate(degree); } //Rotate shape
	virtual void markNewOwner(char color, int owner); //Change shape and neighbor's owners based on color
	virtual void countColors(std::vector<int>& counter); //Count how many shapes of each color are reachable from a shape
	virtual void setTexture(sf::Texture& playerTexture); //Set texture to shape
	virtual void removeTexture(); //Remove texture from shape
	void randomizeColor(int owner); //Change shapes color to a random one
	float getWidth() { return m_shape.getGlobalBounds().width; } //Get shape width
	float getHeight() { return m_shape.getGlobalBounds().height; } //Get shape height
	virtual int getShapeNum() { return 1; } //Returns how many shapes are contained in one Shape object

protected:
	sf::ConvexShape m_shape;
	std::vector<std::shared_ptr<Shape>> m_neighbors; //List of neighbors
	char m_color; //r = red, o = orange, y = yellow, g = green, b = blue, p = purple
	int m_ownership; //Decides who "owns" the shape. 1 = player, 2 = computer, 0 = neutral
	bool m_visited; //Has the shape been visited by the color counter
	void setColor(sf::ConvexShape& shape, int owner, char newColor); //Set new color
	virtual std::shared_ptr<Shape> getShape(int direction) { return  shared_from_this(); } //Get pointer to shape
	void markNewOwnerRecursion(std::shared_ptr<Shape> shape, char color, int owner); //Recursive function for markNewOwner
	void colorCounterRecursion(std::shared_ptr<Shape> shape, char color, std::vector<int>& colorVector); //Recursive function for colorCounter
};


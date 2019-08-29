#pragma once
#include <SFML\Graphics.hpp>

const int BUTTON_TYPE_NUMBER = 8;
const int BUTTON_SIZE = 30;

enum ButtonTypes { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, RESTART, QUIT };

class Buttons
{
public:
	Buttons();
	~Buttons() {}
	char getButtonType(sf::Vector2f pos); //Return type of button pressed (color, restart button etc)
	void draw(sf::RenderWindow& window); //Draw button
	void setColorButtonLocation(int x, int y); //Set location of color buttons
	void setQuitButtonLocation(int x, int y); //Set location of quit button
	void setRestartButtonLocation(int x, int y); //Set location of restart button
	int getButtonSize() { return BUTTON_SIZE; } //Get size of buttons
	void disableButtons(char playerButton, char computerButton); //Disable two color buttons based on player and computer colors
	int numOfColors() { return BUTTON_TYPE_NUMBER - 2; } //Number of color buttons
	void disableAll(); //Disable all color buttons
	void enableAll(); //Re-enable all color buttons

private:
	int m_chosenColor1 = -1, m_chosenColor2 = -1; //Currently disabled buttons
	std::vector<sf::RectangleShape> m_buttons; //Color button vector
	sf::Texture m_restartButton;
	sf::Texture m_quitButton;
	sf::Texture m_disabledButton;
	void createButtons(); //Initialize buttons
	void setButtonColor(int num); //Change button color
	int convertCharToInt(char button); //Get color num from color char
	void addOutline(int position); //Add outline to button
};



#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Shape.h"
#include "Rectangle.h"
#include "TwoTriangles.h"
#include "Buttons.h"

const int DECIMALS = 1000000; //Used for score calculation;
const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 800;
const int BOARD_HEIGHT = 43;
const int BOARD_WIDTH = 22;
const int FONT_SIZE = 23;

class Game
{
public:
	Game();
	void start(); //Prepare the board and then start the game

private:
	bool m_shapeFlashingBool; //Decides when the player-owned shapes flash
	bool m_gameOver; //Shows whether the game has finished
	double m_totalShapes; //Counts how many shapes are in on the board (for scoring purposes)
	std::vector<std::shared_ptr<Shape>> m_shapeVector;
	sf::RenderWindow m_window;
	sf::RectangleShape m_background, m_overlay[4];
	sf::Texture m_ButtonFlashingTexture;
	sf::Text m_playerScoreText;
	sf::Text m_computerScoreText;
	sf::Font m_gameFont;
	Buttons m_buttons;
	char m_playerColor, m_computerColor;
	double m_playerScore, m_computerScore;
	void run(); //Play the game
	void createBoard(); //Create a new playing board
	std::shared_ptr<Shape> createShape(int x, int y); //Create new shape for the board
	void setOverlays(std::vector<std::vector<std::shared_ptr<Shape>>> &matrix); //Place overlays over the board
	void fillNeighborList(std::vector<std::vector<std::shared_ptr<Shape>>> &matrix); //Fill the neighbor list for all the shapes in a matrix
	//Make sure that the initial player and computer colors(and the shapes near them) are all different to prevent a player having a disadvantage at the start
	void colorCorrection(std::vector<std::vector<std::shared_ptr<Shape>>> &matrix, int pX, int pY, int cX, int cY); 
	void fillVector(std::vector<std::vector<std::shared_ptr<Shape>>>& matrix); //Fill the shape vector
	void drawShapes(); //Draw all of the shapes on the board
	void changeColorByOwner(); //Search the shape vector and color each block based on its owner's color
	void colorSelected(char color); //Mark appropriate blocks according to each player's choice and check if there's a winner
	char compChooseColor(char playerColor); //Computer algorithm for choosing the best color
	void flashPlayerShapes(); //Causes the pleyer-owned shapes to flash at regular intervals
	void calculateScore(); //Calculate current score for each player
	std::string createScoreString(std::string tempString, int score); //Edit the string that displays the scores
	void declareWinner(); //Declare who won the game
};


#include "Game.h"
#include <time.h>
#include <typeinfo>

Game::Game()
{
	m_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "6 Colors");
	m_background.setFillColor(sf::Color(128, 128, 128));
	m_background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_ButtonFlashingTexture.loadFromFile("Textures/gray.png");
	m_shapeFlashingBool = false;
	m_gameFont.loadFromFile("Font/8-BIT.TTF");

	m_playerScoreText.setCharacterSize(FONT_SIZE);
	m_computerScoreText.setCharacterSize(FONT_SIZE);
	m_playerScoreText.setFillColor(sf::Color::White);
	m_computerScoreText.setFillColor(sf::Color::White);
	m_playerScoreText.setPosition(sf::Vector2f(20, SCREEN_HEIGHT - FONT_SIZE * 3));
	m_computerScoreText.setPosition(sf::Vector2f(SCREEN_WIDTH / 6 * 5, SCREEN_HEIGHT - FONT_SIZE * 3));
	m_playerScoreText.setFont(m_gameFont);
	m_computerScoreText.setFont(m_gameFont);

	for (int i = 0; i < 4; i++)
		m_overlay[i].setFillColor(m_background.getFillColor());
}

void Game::start()
{
	srand(time(0));
	while (true)
	{
		m_playerScoreText.setString("Player:\n0.000000");
		m_computerScoreText.setString("Computer:\n0.000000");
		m_buttons.enableAll();
		createBoard();
		run();
	}
}

void Game::run()
{
	char buttonPressed = '0';
	sf::Event events;
	sf::Vector2f mousePos;
	sf::Clock clock;
	clock.restart();

	while (m_window.isOpen())
	{
		if (m_window.pollEvent(events))
		{
			if (events.type == sf::Event::Closed || buttonPressed == 'q')
			{
				m_window.close();
				exit(EXIT_SUCCESS);
			}
			if (events.type == sf::Event::MouseButtonReleased)
			{
				mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)); //Get mouse coordinate
				buttonPressed = m_buttons.getButtonType(sf::Vector2f(mousePos)); //Check which button has been pressed
				if (buttonPressed == ' ') //Restart button pressed
					return;
				else if (buttonPressed == '0' || buttonPressed == 'q')
					;
				else
					if (m_gameOver) //Disable color-button presses if the game is over
						continue;
					else
						colorSelected(buttonPressed);
			}
		}
		if (clock.getElapsedTime() >= sf::seconds(1)) //Cause the player-owned shapes to flash
		{
			flashPlayerShapes();
			clock.restart();
		}
		m_window.clear();
		drawShapes();
		m_window.display();
	}
}

void Game::createBoard()
{
	m_totalShapes = 0;
	m_gameOver = false;
	int playerStartX = 0;
	int playerStartY = BOARD_HEIGHT - 1;
	int computerStartY = 0;
	int computerStartX = BOARD_WIDTH - 1;
	std::vector<std::vector<std::shared_ptr<Shape>>> tempMatrix; //The shape matrix
	std::vector<std::shared_ptr<Shape>> tempVector; //Each line in the shape matrix

	m_shapeVector.clear();
	for (int i = 0; i < BOARD_HEIGHT; i++) //Fill matrix with shapes
	{
		tempMatrix.push_back(tempVector);
		for (int k = 0; i % 2 == 0 ? k < BOARD_WIDTH : k < BOARD_WIDTH - 1; k++)
		{
			tempMatrix[i].push_back(createShape(k, i));
			tempMatrix[i].back()->setLocation(k, i);
		}
	}
	fillNeighborList(tempMatrix);
	m_buttons.setColorButtonLocation(SCREEN_WIDTH / 6 * 2,SCREEN_HEIGHT - 50);
	m_buttons.setRestartButtonLocation(0, 0);
	m_buttons.setQuitButtonLocation(m_buttons.getButtonSize() * 1.5, 0);
	tempMatrix[playerStartY][playerStartX]->setOwnership(1);
	tempMatrix[computerStartY][computerStartX]->setOwnership(2);
	m_buttons.disableButtons(tempMatrix[playerStartY][playerStartX]->getColor(), tempMatrix[computerStartY][computerStartX]->getColor());
	colorCorrection(tempMatrix, playerStartX, playerStartY, computerStartX, computerStartY);
	setOverlays(tempMatrix);
	fillVector(tempMatrix);
	for (auto i : m_shapeVector)
		m_totalShapes += i->getShapeNum();
}

std::shared_ptr<Shape> Game::createShape(int x, int y)
{
	int randNum = rand() % 5;
	//Randomly choose if the shape is a square or two triangles (outer rim can only contain squares)
	if (randNum == 0 && x > 0 && x < BOARD_WIDTH - 1 && y > 0 && y < BOARD_HEIGHT - 1) //Randomly choose if the sha
	{
		std::shared_ptr<TwoTriangles> tempShape = std::make_shared<TwoTriangles>();
		return tempShape;
	}
	else
	{
		std::shared_ptr<Rectangle> tempShape = std::make_shared<Rectangle>();
		return tempShape;
	}
}

void Game::setOverlays(std::vector<std::vector<std::shared_ptr<Shape>>> &matrix)
{
	float height = matrix[0][0]->getHeight();
	float width = matrix[0][0]->getWidth();
	m_overlay[0].setSize({ width * BOARD_WIDTH, height / 2 });
	m_overlay[0].setPosition(matrix[0][0]->getPosition());
	m_overlay[0].move({ -width / 2, -height });
	m_overlay[1] = m_overlay[0];
	m_overlay[1].move({ 0, height / 2 * BOARD_HEIGHT });
	m_overlay[2].setSize({ width / 2, height * BOARD_HEIGHT });
	m_overlay[2].setPosition(matrix[0][0]->getPosition());
	m_overlay[2].move({ -width / 2, -height / 2 });
	m_overlay[3] = m_overlay[2];
	m_overlay[3].move({ width / 2 * (BOARD_WIDTH * 2 - 1),0 });
}

void Game::fillNeighborList(std::vector<std::vector<std::shared_ptr<Shape>>>& matrix)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int k = 0; k < matrix[i].size(); k++)
		{
			//Add a neighbor according to the shape's direction(add null pointer if a neighbor doesn't exist in one direction)
			if (i % 2 == 0)
			{
				if (i - 1 >= 0 && k - 1 >= 0)
					matrix[i][k]->addNeighbor(matrix[i - 1][k - 1], 0);
				else
					matrix[i][k]->addNullNeighbor();
				if (i - 1 >= 0 && k < BOARD_WIDTH - 1)
					matrix[i][k]->addNeighbor(matrix[i - 1][k], 1);
				else
					matrix[i][k]->addNullNeighbor();
				if (i + 1 < BOARD_HEIGHT && k - 1 >= 0)
					matrix[i][k]->addNeighbor(matrix[i + 1][k - 1], 2);
				else
					matrix[i][k]->addNullNeighbor();
				if (i + 1 < BOARD_HEIGHT && k < BOARD_WIDTH - 1)
					matrix[i][k]->addNeighbor(matrix[i + 1][k], 3);
				else
					matrix[i][k]->addNullNeighbor();
			}
			else
			{
				if (i - 1 >= 0)
				{
					matrix[i][k]->addNeighbor(matrix[i - 1][k], 0);
					matrix[i][k]->addNeighbor(matrix[i - 1][k + 1], 1);
				}
				else
				{
					matrix[i][k]->addNullNeighbor();
					matrix[i][k]->addNullNeighbor();
				}
				if (i + 1 < BOARD_HEIGHT)
				{
					matrix[i][k]->addNeighbor(matrix[i + 1][k], 2);
					matrix[i][k]->addNeighbor(matrix[i + 1][k + 1], 3);
				}
				else
				{
					matrix[i][k]->addNullNeighbor();
					matrix[i][k]->addNullNeighbor();
				}
			}
		}
}

void Game::colorCorrection(std::vector<std::vector<std::shared_ptr<Shape>>> &matrix, int pX, int pY, int cX, int cY)
{
	std::shared_ptr<Shape> playerNeighbor, computerNeighbor;
	for (auto i : matrix[pY][pX]->getNeighbors())
		if (i != nullptr)
		{
			playerNeighbor = i;
			break;
		}
	for (auto i : matrix[cY][cX]->getNeighbors())
		if (i != nullptr)
		{
			computerNeighbor = i;
			break;
		}
	while (matrix[pY][pX]->getColor() == matrix[cY][cX]->getColor())
	{
		matrix[pY][pX]->randomizeColor(1);
		matrix[cY][cX]->randomizeColor(2);
	}

	while (matrix[pY][pX]->getColor() == playerNeighbor->getColor() || matrix[cY][cX]->getColor() == computerNeighbor->getColor() || playerNeighbor->getColor() == computerNeighbor->getColor() ||
		   matrix[pY][pX]->getColor() == computerNeighbor->getColor() || matrix[cY][cX]->getColor() == playerNeighbor->getColor())
	{
		playerNeighbor->randomizeColor(0);
		computerNeighbor->randomizeColor(0);
	}
}

void Game::fillVector(std::vector<std::vector<std::shared_ptr<Shape>>>& matrix)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int k = 0; k < matrix[i].size(); k++)
			m_shapeVector.push_back(matrix[i][k]);
}

void Game::drawShapes()
{
	m_window.draw(m_background);
	for (auto i : m_shapeVector)
		i->draw(m_window);
	for (auto i : m_overlay)
		m_window.draw(i);
	m_buttons.draw(m_window);
	m_window.draw(m_playerScoreText);
	m_window.draw(m_computerScoreText);
}

void Game::changeColorByOwner()
{
	for (auto i : m_shapeVector)
	{
		i->setColor(1, m_playerColor);
		i->setColor(2, m_computerColor);
	}
}

void Game::colorSelected(char color)
{
	for (auto i : m_shapeVector)
		i->markNewOwner(color, 1);//Change ownership
	m_playerColor = color;
	m_computerColor = compChooseColor(color);
	changeColorByOwner();
	m_buttons.disableButtons(m_playerColor, m_computerColor); //Disable the two colors that both players chose
	calculateScore();
	if (m_playerScore >= 0.5 || m_computerScore >= 0.5)
		declareWinner();
}

char Game::compChooseColor(char playerColor)
{
	int maxValue[2] = { -1,-1 };
	char chosenColor;
	std::vector<int> colorVector = { 0,0,0,0,0,0 };
	for (auto i : m_shapeVector)
		i->countColors(colorVector);
	switch (playerColor) //Ensure that the computer won't pick the same color as the player
	{
		case 'r':
			colorVector[0] = -1;
			break;
		case 'o':
			colorVector[1] = -1;
			break;
		case 'y':
			colorVector[2] = -1;
			break;
		case 'g':
			colorVector[3] = -1;
			break;
		case 'b':
			colorVector[4] = -1;
			break;
		case 'p':
			colorVector[5] = -1;
			break;
	}
	for (int i = 0; i < colorVector.size(); i++) //Check which color appears most often and select it
		if (colorVector[i] > maxValue[0])
		{
			maxValue[0] = colorVector[i];
			maxValue[1] = i;
		}
	switch (maxValue[1])
	{
	case 0:
		chosenColor = 'r';
		break;
	case 1:
		chosenColor = 'o';
		break;
	case 2:
		chosenColor = 'y';
		break;
	case 3:
		chosenColor = 'g';
		break;
	case 4:
		chosenColor = 'b';
		break;
	case 5:
		chosenColor = 'p';
		break;
	}
	for (auto i : m_shapeVector)
	{
		i->markNewOwner(chosenColor, 2);//Change ownership
		i->setVisitedState(false);
	}
	return chosenColor;
}

void Game::flashPlayerShapes()
{
	if (!m_shapeFlashingBool)
	{
		for (auto i : m_shapeVector)
			i->setTexture(m_ButtonFlashingTexture);
		m_shapeFlashingBool = true;
	}
	else
	{
		for (auto i : m_shapeVector)
			i->removeTexture();
		m_shapeFlashingBool = false;
	}
}

void Game::calculateScore()
{
	std::string playerString, computerString;
	double playerTiles = 0, computerTiles = 0;
	for (auto i : m_shapeVector) //Calculate how many tiles are owned by each player
		if (i->getOwnership() == 1)
			playerTiles+= i->getShapeNum();
		else if (i->getOwnership() == 2)
			computerTiles+= i->getShapeNum();
		else if (i->getOwnership() == 2)
		{
			playerTiles++;
			computerTiles++;
		}
	m_playerScore = playerTiles / m_totalShapes;
	m_computerScore = computerTiles / m_totalShapes;
	m_playerScoreText.setString(createScoreString("Player:\n0.", int(playerTiles / m_totalShapes * DECIMALS)));
	m_computerScoreText.setString(createScoreString("Computer:\n0.", int(computerTiles / m_totalShapes * DECIMALS)));
}

std::string Game::createScoreString(std::string tempString, int score)
{
	int zeros = 0;
	int tempScore = score;
	while (tempScore < DECIMALS)
	{
		zeros++;
		tempScore *= 10;
	}
	for (int i = 1; i < zeros; i++)
		tempString += "0";
	tempString += std::to_string(score);
	return tempString;
}

void Game::declareWinner()
{
	m_gameOver = true;
	m_buttons.disableAll();
	if (m_playerScore > m_computerScore)
	{
		m_playerScoreText.setString("Player:\nVictory!");
		m_computerScoreText.setString("Computer:\nDefeat!");
	}
	else
	{
		m_playerScoreText.setString("Player:\nDefeat!");
		m_computerScoreText.setString("Computer:\nVictory!");
	}
}
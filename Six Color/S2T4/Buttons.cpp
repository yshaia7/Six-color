#include "Buttons.h"

Buttons::Buttons()
{
	createButtons();
}

void Buttons::createButtons()
{
	m_buttons.resize(BUTTON_TYPE_NUMBER);
	for (int place = 0; place < BUTTON_TYPE_NUMBER; place++)
	{
		setButtonColor(place);
		m_buttons[place].setSize({ float(BUTTON_SIZE),float(BUTTON_SIZE) });
	}

	m_restartButton.loadFromFile("Textures/restart.png");
	m_quitButton.loadFromFile("Textures/quit.png");
	m_disabledButton.loadFromFile("Textures/x.png");
	
	m_buttons[6].setTexture(&m_restartButton);
	m_buttons[7].setTexture(&m_quitButton);

}

char Buttons::getButtonType(sf::Vector2f pos)
{
	for (int place = 0; place < BUTTON_TYPE_NUMBER; place++)
	{
		if (m_buttons[place].getGlobalBounds().contains(pos.x,pos.y))
		{
			if (place == m_chosenColor1 || place == m_chosenColor2)
				return '0';
			if (place == RED)
				return 'r';
			else if (place == ORANGE)
				return 'o';
			else if (place == YELLOW)
				return 'y';
			else if (place == GREEN)
				return 'g';
			else if (place == BLUE)
				return 'b';
			else if (place == PURPLE)
				return 'p';
			else if (place == RESTART)
				return ' ';
			else if (place == QUIT)
				return 'q';
		}			
	}
	return '0';
}

void Buttons::draw(sf::RenderWindow& window)
{
	for (int place = 0; place < BUTTON_TYPE_NUMBER; place++)
		window.draw(m_buttons[place]);
}


void Buttons::setButtonColor(int num)
{
	switch(num)
	{
		case RED:
		{
			m_buttons[num].setFillColor(sf::Color(255, 0, 0));
			break;
		}
		case ORANGE:
		{
			m_buttons[num].setFillColor(sf::Color(255, 106, 0));
			break;
		}
		case YELLOW:
		{
			m_buttons[num].setFillColor(sf::Color(255, 244, 78));
			break;
		}
		case GREEN:
		{
			m_buttons[num].setFillColor(sf::Color(78, 255, 0));
			break;
		}
		case BLUE:
		{
			m_buttons[num].setFillColor(sf::Color(0, 38, 255));
			break;
		}
		case PURPLE:
		{
			m_buttons[num].setFillColor(sf::Color(255, 0, 220));
			break;
		}
	}
	addOutline(num);
}

void Buttons::setColorButtonLocation(int x, int y)
{
	for (int i = 0; i < BUTTON_TYPE_NUMBER - 2; i++)
		m_buttons[i].setPosition(sf::Vector2f(x + i * BUTTON_SIZE * 1.5, y));
}

void Buttons::setQuitButtonLocation(int x, int y)
{
	m_buttons[6].setPosition(sf::Vector2f(x, y));
}

void Buttons::setRestartButtonLocation(int x, int y)
{
	m_buttons[7].setPosition(sf::Vector2f(x, y));
}

void Buttons::disableButtons(char playerButton, char computerButton)
{
	if (m_chosenColor1 != -1) //Disable previous markings
	{
	m_buttons[m_chosenColor1].setTexture(NULL); 
	m_buttons[m_chosenColor2].setTexture(NULL);
	}
	m_chosenColor1 = convertCharToInt(playerButton);
	m_chosenColor2 = convertCharToInt(computerButton);
	m_buttons[m_chosenColor1].setTexture(&m_disabledButton);
	m_buttons[m_chosenColor2].setTexture(&m_disabledButton);
}

int Buttons::convertCharToInt(char button)
{
	switch (button)
	{
		case 'r':
			return RED;
		case 'o':
			return ORANGE;
		case 'y':
			return YELLOW;
		case 'g':
			return GREEN;
		case 'b':
			return BLUE;
		case 'p':
			return PURPLE;
	}
}

void Buttons::addOutline(int position)
{
	m_buttons[position].setOutlineColor(sf::Color::Black);
	m_buttons[position].setOutlineThickness(1);
}

void Buttons::disableAll()
{
	for (int i = 0; i < numOfColors(); i++)
		m_buttons[i].setTexture(&m_disabledButton);
}

void Buttons::enableAll()
{
	for (int i = 0; i < numOfColors(); i++)
		m_buttons[i].setTexture(NULL);
}
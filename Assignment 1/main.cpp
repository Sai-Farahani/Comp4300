#include <iostream>
#include <fstream>
#include <vector>

#include "SFML/Graphics.hpp"

// class delcarations
class Shape
{
public:
	Shape(std::string name, float posX, float posY, float speedX, float speedY, sf::Color color, bool isCircle)
		: m_name(name), m_posX(posX), m_posY(posY), m_speedX(speedX), m_speedY(speedY), m_color(color), m_isCircle(isCircle)
	{

	}

	virtual sf::Vector2f getPos()
	{
		return sf::Vector2f(m_posX, m_posY);
	}

	void setPos(sf::Vector2f newPos)
	{
		m_posX = newPos.x;
		m_posY = newPos.y;
	}

	sf::Vector2f getSpeed()
	{
		return sf::Vector2f(m_speedX, m_speedY);
	}

	void setSpeed(sf::Vector2f speed)
	{
		m_speedY = speed.y;
		m_speedX = speed.x;
	}

	bool isCircle()
	{
		return m_isCircle;
	}

	sf::Color getColor()
	{
		return m_color;
	}

	std::string getName()
	{
		return m_name;
	}

	const virtual sf::CircleShape getCircle() = 0;
	const virtual sf::RectangleShape getRectangle() = 0;
private:
	float m_posX, m_posY;
	float m_speedX, m_speedY;
	bool m_isCircle;
	std::string m_name;
	sf::Color m_color;
};

class Circle : public Shape
{
public:
	Circle(std::string name, float posX, float posY, float speedX, float speedY, sf::Color color, bool isCircle, float radius)
		: Shape(name, posX, posY, speedX, speedY, color, isCircle), m_circ(radius)
	{
		m_circ.setFillColor(getColor());
		m_circ.setPosition(getPos());
	}

	sf::Vector2f getSize()
	{
		return sf::Vector2f(m_circ.getRadius() * 2, m_circ.getRadius() * 2);
	}


	void update(sf::Vector2f newPos)
	{
		m_circ.setPosition(newPos);
	}

	const virtual sf::CircleShape getCircle() override
	{
		return m_circ;
	}

private:
	sf::CircleShape m_circ;
	const virtual sf::RectangleShape getRectangle() override
	{
		std::cerr << "Not a Rectangle\n";
		exit(-4);
	}
};

class Rectangle : public Shape
{
public:
	Rectangle(std::string name, float posX, float posY, float speedX, float speedY, sf::Color color, bool isCircle, float height, float width)
		: Shape(name, posX, posY, speedX, speedY, color, isCircle), m_width(width), m_height(height)
	{
		m_rect.setFillColor(getColor());
		m_rect.setPosition(getPos());
		m_rect.setSize(sf::Vector2f(m_width, m_height));
	}
	const virtual sf::RectangleShape getRectangle() override
	{
		return m_rect;
	}

	void update(sf::Vector2f newPos)
	{
		m_rect.setPosition(newPos);
	}

	sf::Vector2f getSize()
	{
		return sf::Vector2f(m_width, m_height);
	}

private:
	sf::RectangleShape m_rect;
	float m_width, m_height;
	const virtual sf::CircleShape getCircle() override
	{
		std::cerr << "Not a Circle\n";
		exit(-5);
	}

};

// func declarations
void init(int& width, int& height, sf::Font& font, int& fontSize, sf::Color fontColor, std::vector<Circle>& circles, std::vector<Rectangle>& rectangles);
void draw(sf::RenderWindow& window, std::vector<Circle>& circles, std::vector<Rectangle>& rects);
void drawFont(sf::RenderWindow& window, std::vector<Circle>& circles, std::vector<Rectangle>& rects, sf::Font& myFont, int& fontsize);
void update(std::vector<Circle>& circles, std::vector<Rectangle>& rects, int& width, int& height);

int main(int argc, int* argv[])
{
	int width, height, fontsize;
	sf::Font myFont;
	sf::Color fontColor;
	std::vector<Circle> circles;
	std::vector<Rectangle> rects;

	init(width, height, myFont, fontsize, fontColor, circles, rects);


	sf::RenderWindow window(sf::VideoMode(width, height), "Test");

	while (window.isOpen())
	{	
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		draw(window, circles, rects);
		drawFont(window, circles, rects, myFont, fontsize);
		update(circles, rects, width, height);
		window.display();
	}

	return 0;
}

void getResolution(int& width, int& height, std::ifstream& fin)
{
	std::string type;
	fin >> type;
	if (type == "Window")
	{
		fin >> width;
		fin >> height;
	}
	else
	{
		std::cerr << "Couldn't find dimenstions in file\n";
		exit(-1);
	}
}

void getFont(sf::Font& font, int& fontSize, sf::Color fontColor, std::ifstream& fin)
{
	std::string type;
	fin >> type;
	if (type == "Font")
	{
		fin >> type;
		if (!font.loadFromFile(type))
		{
			std::cerr << "Couldn't load font! \n";
			exit(-3);
		}
		fin >> fontSize;
		unsigned int r, g, b;
		fin >> r;
		fin >> g;
		fin >> b;
		fontColor = sf::Color(r, g, b);
	}
	else
	{
		std::cerr << "Couldn't find Font in file\n";
		exit(-2);
	}
}

void getShapes(std::vector<Circle>& circles, std::vector<Rectangle>& rects, std::ifstream& fin)
{
	std::string type;
	while (fin >> type)
	{
		if (type == "Circle")
		{
			std::string name;
			float posX, posY;
			float speedX, speedY;
			unsigned int r, g, b;
			float radius;
			fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> radius;
			std::cout << "X: " << posX << " Y: " << posY << std::endl;
			Circle circ(name, posX, posY, speedX, speedY, sf::Color(r, g, b), true, radius);
			circles.push_back(circ);
		}
		else if (type == "Rectangle")
		{
			std::string name;
			float posX, posY;
			float speedX, speedY;
			unsigned int r, g, b;
			float width, height;
			fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> height >> width;
			Rectangle rect(name, posX, posY, speedX, speedY, sf::Color(r, g, b), true, width, height);
			rects.push_back(rect);
		}
		else
		{
			std::cerr << "Unkown type: " << type << "\n";
		}
	}
}

void init(int& width, int& height, sf::Font& font, int& fontSize, sf::Color fontColor, std::vector<Circle>& circles, std::vector<Rectangle>& rectangles)
{
	std::string filename = "config.txt";
	std::ifstream fin{filename};
	
	getResolution(width, height, fin);
	
	getFont(font, fontSize, fontColor, fin);

	getShapes(circles, rectangles, fin);

	std::cout << "Finished :3\n";
}

void draw(sf::RenderWindow& window, std::vector<Circle>& circles, std::vector<Rectangle>& rects)
{
	window.clear();
	for (auto& elem : circles)
	{
		window.draw(elem.getCircle());
	}
	for (auto& elem : rects)
	{
		window.draw(elem.getRectangle());
	}
}

void drawFont(sf::RenderWindow& window, std::vector<Circle>& circles, std::vector<Rectangle>& rects, sf::Font& font, int& fontsize)
{
	for (auto& elem : circles)
	{
		sf::Text txt(elem.getName(), font, fontsize);
		txt.setPosition((elem.getPos().x + elem.getSize().x / 2) - txt.getLocalBounds().width / 2, (elem.getPos().y + elem.getSize().y / 2) - txt.getLocalBounds().height / 2);
		window.draw(txt);
	}
	for (auto& elem : rects)
	{
		sf::Text txt(elem.getName(), font, fontsize);
		txt.setPosition((elem.getPos().x + elem.getSize().x / 2) - txt.getLocalBounds().width / 2, (elem.getPos().y + elem.getSize().y / 2) - txt.getLocalBounds().height / 2);
		window.draw(txt);
	}
}

void update(std::vector<Circle>& circles, std::vector<Rectangle>& rects, int& width, int& height)
{
	for (auto& elem : circles)
	{
		elem.setPos(sf::Vector2f(elem.getPos().x + elem.getSpeed().x, elem.getPos().y + elem.getSpeed().y));
		elem.update(elem.getPos());
		if (elem.getPos().x + elem.getSize().x > width || elem.getPos().x < 0)
		{
			elem.setSpeed(sf::Vector2f(-elem.getSpeed().x, elem.getSpeed().y));
		}
		if (elem.getPos().y + elem.getSize().y > height || elem.getPos().y < 0)
		{
			elem.setSpeed(sf::Vector2f(elem.getSpeed().x, -elem.getSpeed().y));
		}
	}
	for (auto& elem : rects)
	{
		elem.setPos(sf::Vector2f(elem.getPos().x + elem.getSpeed().x, elem.getPos().y + elem.getSpeed().y));
		elem.update(elem.getPos());

		if (elem.getPos().x + elem.getSize().x > width || elem.getPos().x < 0)
		{
			elem.setSpeed(sf::Vector2f(-elem.getSpeed().x, elem.getSpeed().y));
		}
		if (elem.getPos().y + elem.getSize().y > height || elem.getPos().y < 0)
		{
			elem.setSpeed(sf::Vector2f(elem.getSpeed().x, -elem.getSpeed().y));
		}
	}	
}
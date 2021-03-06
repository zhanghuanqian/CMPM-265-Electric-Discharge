#pragma once
#include <SFML\Graphics.hpp>
#include "Form.h"
#include "ElectricArc.h"

using namespace sf;
using namespace std;

class ElectricArcDemo: public Form
{
private:
	Text instruction1, instruction2;
	Font font;
	Display nextDisplay;
	ElectricArc electricArc;
public:
	ElectricArcDemo(int windowWidth = 100, int windowHeight = 100) :Form(windowWidth, windowHeight),
	nextDisplay(Display::Demo),
	electricArc(Vector2f(400,50),90,600,5,0)
	{
		srand(time(NULL));
		int amplitude = rand() % Amplitude_Margin + Amplitude_Base;
		electricArc.setAmplitude(amplitude);
		electricArc.setNoiseSeed(rand());
		electricArc.buildArc();
		font.loadFromFile("Tinos-Regular.ttf");
		instruction1.setFont(font);
		instruction1.setString("Press Enter to generate another electric arc.");
		instruction1.setCharacterSize(Instruction_Character_Size);
		instruction1.setFillColor(Color::Red);
		instruction1.setPosition(Menu_Instruction_X, Menu_Instruction_Y);
		instruction2.setFont(font);
		instruction2.setString("Press Escape to return to menu.");
		instruction2.setCharacterSize(Instruction_Character_Size);
		instruction2.setFillColor(Color::Red);
		instruction2.setPosition(Menu_Instruction_X, Menu_Instruction_Y + 30);
	}
	virtual void processEvent(Event event, RenderWindow& window) override;
	virtual void update(float deltaTime) override;
	virtual void render(RenderWindow &window) override;
	virtual Display next() override;
};

void ElectricArcDemo::processEvent(Event event, RenderWindow& window)
{
	if (Keyboard::isKeyPressed(Keyboard::Return)) {
		electricArc.setNoiseSeed(rand());
		int amplitude = rand() % Amplitude_Margin + Amplitude_Base;
		electricArc.setAmplitude(amplitude);
		electricArc.buildArc();
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		nextDisplay = Display::Menu;
	}
}

void ElectricArcDemo::update(float deltaTime)
{
	
}

void ElectricArcDemo::render(RenderWindow& window)
{
	electricArc.draw(window);
	window.draw(instruction1);
	window.draw(instruction2);
}

Display ElectricArcDemo::next()
{
	return nextDisplay;
}


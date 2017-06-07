#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <noise/noise.h>
#include "Matrix.h"
#include "Constants.h"

using namespace std;
using namespace sf;
using namespace noise;

class ElectricArc
{
private:
	Vector2f sourcePoint,targetPoint;
	float direction, length;
	vector<Vector2f> arcPath;
	float arcThickness, amplitude;
	vector<shared_ptr<ElectricArc>> branches;
	Color color;
	bool persistent,continuous,branching;
	float duration;
	int fadingRate;
	module::Perlin noiseGenerator;
public:
	ElectricArc(Vector2f s = Vector2f(0, 0), float d = 0, float l = 10, float a = 1, float am = 50, bool p = true, float du=0,int f=0) : sourcePoint(s),
	targetPoint(s),
	direction(d),
	length(l),
	arcThickness(a),
	amplitude(am),
	color(Color::White),
	persistent(p),
	continuous(false),
	branching(true),
	duration(du),
	fadingRate(f)
	{
		noiseGenerator.SetOctaveCount(5);
		noiseGenerator.SetFrequency(2);
		noiseGenerator.SetPersistence(0.5);
	}
	void buildArc();
	void setNoiseSeed(int i) { noiseGenerator.SetSeed(i); }
	void setAmplitude(float a) { amplitude = a; }
	void setTargetPoint(Vector2f t) { targetPoint = t; }
	void setContinuous(bool c) { continuous = c; }
	void setBranching(bool b) { branching = b; }
	void draw(RenderWindow &window);
	void update(float deltaTime);
	bool isVisible() const { return color.a > 0; }
};

void ElectricArc::buildArc()
{
	arcPath.clear();
	branches.clear();
	float x = 0;
	while (x <= length)
	{
		arcPath.push_back(Vector2f(x, noiseGenerator.GetValue(x / Noise_X_Input_Dividend, Noise_Y_Input, 0) * amplitude));
		x += 1;
	}

	Matrix rotationMatrix(cos(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	for (int i = 0; i < arcPath.size(); ++i) {
		arcPath[i] = rotationMatrix*arcPath[i] + sourcePoint;

		int p = rand() % Branching_Probability;
		if (p == 0 && arcThickness>Branching_Threshold)
		{
			float branchDirection = rand() % Branching_Angle_Margin + Branching_Angle_Base;
			branchDirection = branchDirection*(rand() % 2 == 0 ? 1 : -1);
			branchDirection += direction;
			float branchLengthDecrease = (rand() % Branch_Length_Decrease_Margin + Branch_Length_Decrease_Base) / 10.f;
			float branchThicknessDecrease = (rand() % Branch_Thickness_Decrease_Margin + Branch_Thickness_Decrease_Base) / 10.f;
			int branchAmplitude = rand() % Amplitude_Margin + Amplitude_Base;
			shared_ptr<ElectricArc> arc = make_shared<ElectricArc>(arcPath[i], branchDirection, length*branchLengthDecrease, arcThickness*branchThicknessDecrease,branchAmplitude,persistent,duration,fadingRate);
			arc->setNoiseSeed(time(NULL));
			arc->buildArc();
			branches.push_back(arc);
		}
	}
}

void ElectricArc::draw(RenderWindow& window)
{
	for(Vector2f p:arcPath)
	{
		CircleShape c(arcThickness);
		c.setOrigin(arcThickness, arcThickness);
		c.setPosition(p);
		c.setFillColor(color);
		c.setOutlineColor(color);
		window.draw(c);
	}
	for (shared_ptr<ElectricArc> a : branches)
		a->draw(window);
}

void ElectricArc::update(float deltaTime)
{
	if(!persistent)
	{
		duration -= deltaTime;
		if(duration<=0)
		{
			int alpha = color.a - fadingRate*deltaTime;
			if (alpha < 0)
				alpha = 0;
			color = Color(color.r, color.g, color.b, alpha);
		}
		for (shared_ptr<ElectricArc> a : branches)
			a->update(deltaTime);
	}
}
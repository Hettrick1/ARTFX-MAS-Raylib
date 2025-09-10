#pragma once
#include "raylib.h"

class Obstacle
{
private:
	Rectangle mRect;
public:
	Obstacle();
	~Obstacle();
	void Draw();
};
#pragma once
#include "raylib.h"

class Obstacle
{
private:
	Rectangle mRect;
public:
	Obstacle(const Rectangle& pRectangle);
	~Obstacle();
	void Draw();

	inline Rectangle GetRectangle() const { return mRect; }
	inline Vector2 GetPosition() const { return Vector2{ mRect.x, mRect.y }; }
	inline Vector2 GetSize() const { return Vector2{ mRect.width, mRect.height }; }
};
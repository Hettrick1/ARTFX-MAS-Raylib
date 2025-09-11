#include "Obstacle.h"

Obstacle::Obstacle(const Rectangle& pRectangle)
	: mRect(pRectangle)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Draw()
{
	Vector2 origin = Vector2{ (float)(mRect.width * 0.5), (float)(mRect.height * 0.5) };
	DrawRectanglePro(mRect, origin, 0, BLACK);
}

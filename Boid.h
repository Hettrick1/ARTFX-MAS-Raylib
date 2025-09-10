#pragma once
#include "raylib.h"
#include <vector>

class Obstacle;

class Boid
{
private:
	Rectangle mRect;
	Vector2 mVelocity;
	float mMaxSpeed = 50;
	float mRotation = 0;
	float mMinimumDistance = 50;
	static int boidsCount;
	int mId;
public:
	Boid(Vector2 pPosition, float pInitialRotation);
	~Boid();

	void Move(std::vector<Boid*>& pBoids);

	Vector2 Separate(std::vector<Boid*>& pOthers);
	Vector2 AvoidObstacles(std::vector<Obstacle*> pObstacles);
	Vector2 Align(std::vector<Boid*> pOthers);
	Vector2 Group(std::vector<Boid*> pOthers);

	inline Vector2 GetVelocity() const { return mVelocity; }
	inline void SetVelocity(Vector2 pVelocity) { mVelocity = pVelocity; }

	inline Vector2 GetPosition() const { return Vector2{ mRect.x, mRect.y }; }

	inline int GetID() const { return mId; }

	void Draw();

	Vector2 Normalize(const Vector2& velocity);
};
#pragma once
#include "raylib.h"
#include <vector>

class Obstacle;

class Boid
{
private:
	Rectangle mRect;
	Vector2 mVelocity;
	float mMaxSpeed = 250;
	float mRotation = 0;
	float mMinimumDistance = 35;
	static int boidsCount;
	int mId;
	Texture mTexture;
	Color mColor;
public:
	Boid(const Vector2& pInitialPosition, const float& pInitialSize, float pInitialRotation, const Color& color, const Texture& pTexture);
	~Boid();

	void Move(const std::vector<Boid*>& pBoids, const std::vector<std::vector<Boid*>>& pAllOtherBoids, const std::vector<Obstacle*>& pObstacles);

	Vector2 Separate(const Boid* pOthers);
	Vector2 AvoidScreenBorder();
	Vector2 AvoidObstacles(const std::vector<Obstacle*>& pObstacles);
	Vector2 Align(const Boid* pOthers);
	Vector2 Group(const Boid* pOthers);
	Vector2 AvoidMouse();
	Vector2 AvoidOtherBoids(const std::vector<std::vector<Boid*>>&);


	inline Vector2 GetVelocity() const { return mVelocity; }
	inline void SetVelocity(Vector2 pVelocity) { mVelocity = pVelocity; }

	inline Vector2 GetPosition() const { return Vector2{ mRect.x, mRect.y }; }
	inline Vector2 GetSize() const { return Vector2{ mRect.width, mRect.height }; }


	inline int GetID() const { return mId; }

	void Draw();

	Vector2 Normalize(const Vector2& vector);
	float Length(const Vector2& vector);
	float ClampAngle(float& angle);
	float DotProduct(const Vector2& vector1, const Vector2& vector2);
};
#include "Boid.h"
#include "Obstacle.h"
#include <algorithm>

int Boid::boidsCount = 0;

Boid::Boid(Vector2 pPosition, float pInitialRotation)
    : mRect(Rectangle{pPosition.x, pPosition.y, 10, 10}), mRotation(pInitialRotation)
{
    mVelocity.x = mMaxSpeed * cos(mRotation);
    mVelocity.y = mMaxSpeed * sin(mRotation);
    mId = boidsCount;
    boidsCount++;
}

Boid::~Boid()
{
}

void Boid::Move(std::vector<Boid*>& pBoids, std::vector<Obstacle*> pObstacles)
{
    Vector2 steering = Separate(pBoids);

    Vector2 steeringScreen = AvoidScreenBorder();
    Vector2 steeringObstacles = AvoidObstacles(pObstacles);

    mVelocity.x += (steering.x * 0.8) + (steeringScreen.x * 0.2) + (steeringObstacles.x * 0.2);
    mVelocity.y += (steering.y * 0.8) + (steeringScreen.y * 0.2) + (steeringObstacles.y * 0.2);

    Vector2 desiredVelocity = Normalize(mVelocity);

    float desiredAngle = atan2(desiredVelocity.y, desiredVelocity.x);

    float angleDiff = desiredAngle - mRotation;

    ClampAngle(angleDiff);

    float turnFactor = 100.0f;
    if (angleDiff > turnFactor) 
    {
        angleDiff = turnFactor;
    }
    if (angleDiff < -turnFactor) 
    {
        angleDiff = -turnFactor;
    }

    mRotation += angleDiff;

    mVelocity.x = cos(mRotation) * mMaxSpeed;
    mVelocity.y = sin(mRotation) * mMaxSpeed;

    mRect.x += mVelocity.x * GetFrameTime();
    mRect.y += mVelocity.y * GetFrameTime();
}

Vector2 Boid::Separate(std::vector<Boid*>& pOthers)
{
    Vector2 steering = { 0.0f, 0.0f };
    int count = 0;
    float minDistSq = mMinimumDistance * mMinimumDistance;
    const float EPS = 1e-6f;

    for (const auto& boid : pOthers)
    {
        if (boid->GetID() == mId)
        {
            continue;
        }

        Vector2 diff = { GetPosition().x - boid->GetPosition().x, GetPosition().y - boid->GetPosition().y };

        float distSq = diff.x * diff.x + diff.y * diff.y;
        if (distSq < minDistSq && distSq > EPS)
        {      
            steering.x += diff.x * (1 / distSq * 10);
            steering.y += diff.y * (1 / distSq * 10);;
            count++;
        }
    }

    if (count > 0)
    {
        steering.x /= (float)count;
        steering.y /= (float)count;
        steering = Normalize(steering);
    }
    return steering;
}

Vector2 Boid::AvoidScreenBorder()
{
    int Xmin = 100, Xmax = 980, Ymin = 100, Ymax = 620; // screen size - 100
    Vector2 steering = { 0.0f, 0.0f };

    if (mRect.x < Xmin)
    {
        steering.x = 10;
    }
    else if (mRect.x > Xmax)
    {
        steering.x = -10;
    }
    if (mRect.y < Ymin)
    {
        steering.y = 10;
    }
    else if (mRect.y > Ymax)
    {
        steering.y = -10;
    }

    return steering;
}

Vector2 Boid::AvoidObstacles(std::vector<Obstacle*> pObstacles)
{
    return Vector2{ 0.0f, 0.0f };
}

Vector2 Boid::Align(std::vector<Boid*> pOthers)
{
    return Vector2();
}

Vector2 Boid::Group(std::vector<Boid*> pOthers)
{
    return Vector2();
}

void Boid::Draw()
{
    Vector2 origin = Vector2{ (float)(mRect.width * 0.5), (float)(mRect.height * 0.5) };

    DrawRectanglePro(mRect, origin, mRotation * RAD2DEG, WHITE);

    //DrawCircleLines(mRect.x, mRect.y, mMinimumDistance, RED);
}

Vector2 Boid::Normalize(const Vector2& vector)
{
    if (vector.x == 0 && vector.y == 0)
    {
        return vector;
    }
    float newX = vector.x / sqrt(vector.x * vector.x + vector.y * vector.y);
    float newY = vector.y / sqrt(vector.x * vector.x + vector.y * vector.y);
    return Vector2{ newX, newY };
}

float Boid::Length(const Vector2& vector)
{
    float length = vector.x / sqrt(vector.x * vector.x + vector.y * vector.y);
    return length;
}

float Boid::ClampAngle(float& angle)
{
    while (angle > PI)  angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}
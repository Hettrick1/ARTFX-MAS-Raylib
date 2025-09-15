#include "Boid.h"
#include "Obstacle.h"
#include <algorithm>
#include <iostream>

int Boid::boidsCount = 0;

Boid::Boid(const Vector2& pInitialPosition, const float& pInitialSize, float pInitialRotation, const Color& pColor, const Texture& pTexture)
    : mRect(Rectangle{ pInitialPosition.x, pInitialPosition.y, pInitialSize, pInitialSize}), mRotation(pInitialRotation), mColor(pColor), mTexture(pTexture)
{

    mMinimumDistance = mRect.width * 1.5;
    mMaxSpeed *= (1 / pInitialSize) * 20;

    mVelocity.x = mMaxSpeed * cos(mRotation);
    mVelocity.y = mMaxSpeed * sin(mRotation);
    mId = boidsCount;
    boidsCount++;
    mTexture.width = mRect.width;
    mTexture.height = mRect.height;
}

Boid::~Boid()
{
    UnloadTexture(mTexture);
}

void Boid::Move(const std::vector<Boid*>& pBoids, const std::vector<std::vector<Boid*>>& pAllOtherBoids, const std::vector<Obstacle*>& pObstacles)
{
    Vector2 separateForce = Separate(pBoids);

    Vector2 separateScreen = AvoidScreenBorder();
    Vector2 separateObstacle = AvoidObstacles(pObstacles);
    Vector2 groupForce = Group(pBoids);
    Vector2 allignForce = Align(pBoids);
    Vector2 mouseForce = AvoidMouse();
    Vector2 otherBoidsForce = AvoidOtherBoids(pAllOtherBoids);

    mVelocity.x += (separateForce.x * 1.2) + (separateScreen.x * 0.3) + (separateObstacle.x * 0.5);
    mVelocity.y += (separateForce.y * 1.2) + (separateScreen.y * 0.3) + (separateObstacle.y * 0.5);

    mVelocity.x += groupForce.x * 0.05;
    mVelocity.y += groupForce.y * 0.05;

    mVelocity.x += allignForce.x * 0.125;
    mVelocity.y += allignForce.y * 0.125;

    mVelocity.x += otherBoidsForce.x * 0.5;
    mVelocity.y += otherBoidsForce.y * 0.5;

    Vector2 desiredVelocity = Normalize(mVelocity);

    float desiredAngle = atan2(desiredVelocity.y, desiredVelocity.x);

    float angleDiff = desiredAngle - mRotation;

    ClampAngle(angleDiff);

    float turnFactor = 0.8;
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

Vector2 Boid::Separate(const std::vector<Boid*>& pOthers)
{
    Vector2 separation = { 0.0f, 0.0f };
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
            separation.x += (1 / (diff.x == 0 ? 1 : diff.x)) * mMaxSpeed;
            separation.y += (1 / (diff.y == 0 ? 1 : diff.y)) * mMaxSpeed;
        }
    }
    if (Length(separation) > EPS)
    {
        Normalize(separation);
    }
    return separation;
}

Vector2 Boid::AvoidScreenBorder()
{
    int Xmin = 200, Xmax = 1720, Ymin = 200, Ymax = 880; // screen size - 100
    Vector2 steering = { 0.0f, 0.0f };

    if (mRect.x < Xmin)
    {
        steering.x = 40;
    }
    else if (mRect.x > Xmax)
    {
        steering.x = -40;
    }
    if (mRect.y < Ymin)
    {
        steering.y = 40;
    }
    else if (mRect.y > Ymax)
    {
        steering.y = -40;
    }

    return steering;
}

Vector2 Boid::AvoidObstacles(const std::vector<Obstacle*>& pObstacles)
{
    Vector2 steering = { 0.0f, 0.0f };
    float minDistSq = mMaxSpeed * mMaxSpeed * 1.2;
    const float EPS = 1e-6f;

    for (auto& obstacle : pObstacles) {
        Vector2 diff = { GetPosition().x - obstacle->GetPosition().x , GetPosition().y - obstacle->GetPosition().y };
        float distSq = diff.x * diff.x + diff.y * diff.y;

        float obstacleRadius = sqrt(obstacle->GetSize().x * 0.5 * obstacle->GetSize().x * 0.5 + obstacle->GetSize().y * 0.5 * obstacle->GetSize().y * 0.5);

        float safeRadius = mMaxSpeed * 0.2 + obstacleRadius;

        float safeRadiusSq = safeRadius * safeRadius;

        if (distSq < safeRadiusSq && distSq > EPS)
        {
            float dist = sqrt(distSq);
            diff.x /= dist;
            diff.y /= dist;

            float strengh = (safeRadius - dist) / safeRadius;

            steering.x += diff.x * strengh * 5000;
            steering.y += diff.y * strengh * 5000;
        }
    }
    return steering;
}

Vector2 Boid::Align(const std::vector<Boid*>& pOthers)
{
    Vector2 pV = { 0, 0 };
    for (const auto& boid : pOthers)
    {
        Vector2 diff = { GetPosition().x - boid->GetPosition().x, GetPosition().y - boid->GetPosition().y };

        if (boid->GetID() == mId || Length(diff) > mRect.width + 150) //  || DotProduct(GetPosition(), boid->GetPosition()) > -0.5
        {
            continue;
        }
        
        pV.x += boid->GetVelocity().x;
        pV.y += boid->GetVelocity().y;
    }
    pV.x /= pOthers.size() - 1;
    pV.y /= pOthers.size() - 1;

    return Vector2{ (pV.x - mVelocity.x), (pV.y - mVelocity.y)};
}

Vector2 Boid::Group(const std::vector<Boid*>& pOthers)
{
    Vector2 cM = { 0, 0 };
    for (const auto& boid : pOthers)
    {
        if (boid->GetID() == mId ) // || DotProduct(GetPosition(), boid->GetPosition()) > -0.5
        {
            continue;
        }
        cM.x += boid->GetPosition().x;
        cM.y += boid->GetPosition().y;
    }
    cM.x /= pOthers.size() - 1;
    cM.y /= pOthers.size() - 1;

    return Vector2{(cM.x - mRect.x), (cM.y - mRect.y)};
}

Vector2 Boid::AvoidMouse()
{
    Vector2 force = { 0, 0 };

    Vector2 mouse = GetMousePosition();

    Vector2 diff = { GetPosition().x - mouse.x, GetPosition().y - mouse.y };

    if (Length(diff) > 200)
    {
        return force;
    }

    force.x += (1 / diff.x) * mMaxSpeed * mMaxSpeed;
    force.y += (1 / diff.y) * mMaxSpeed * mMaxSpeed;

    return force;
}

Vector2 Boid::AvoidOtherBoids(const std::vector<std::vector<Boid*>>& pOthers)
{
    Vector2 force = { 0,0 };
    float minDistSq = mMinimumDistance * mMinimumDistance;
    minDistSq *= 50;
    const float EPS = 1e-6f;

    Vector2 cM = { 0, 0 };

    for (auto& other : pOthers)
    {
        if (other[0][0].GetSize().x < GetSize().x)
        {
            continue;
        }
        float otherSize = 0;
        for (auto boid : other)
        {
            cM.x += boid->GetPosition().x;
            cM.y += boid->GetPosition().y;
            otherSize++;
        }
        cM.x /= otherSize;
        cM.y /= otherSize;
        
        Vector2 diff = { GetPosition().x - cM.x, GetPosition().y - cM.y };
        float distSq = diff.x * diff.x + diff.y * diff.y;

        if (distSq < minDistSq && distSq > EPS)
        {
            force.x += (1 / (diff.x == 0 ? 1 : diff.x)) * mMaxSpeed * 500;
            force.y += (1 / (diff.y == 0 ? 1 : diff.y)) * mMaxSpeed * 500;
        }
    }
    return force;
}

void Boid::Draw()
{
    Vector2 origin = Vector2{ (float)(mRect.width * 0.5), (float)(mRect.height * 0.5) };

    DrawTexturePro(mTexture, { 0,0, mRect.width, mRect.height }, mRect, origin, mRotation * RAD2DEG + 180, mColor);
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
    float length = sqrt(vector.x * vector.x + vector.y * vector.y);
    return length;
}

float Boid::ClampAngle(float& angle)
{
    while (angle > PI)  angle -= 2 * PI;
    while (angle < -PI) angle += 2 * PI;
    return angle;
}

float Boid::DotProduct(const Vector2& vector1, const Vector2& vector2)
{
    Vector2 dist = { vector2.x - vector1.x,  vector2.y - vector1.y };
    float dot = vector1.x * vector2.x + vector1.y * vector2.y;
    return dot / Length(dist);
}

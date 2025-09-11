#include "Boid.h"
#include "Obstacle.h"
#include <algorithm>

int Boid::boidsCount = 0;

Boid::Boid(const Vector2& pInitialPosition, float pInitialRotation, const Color& pColor, const Texture& pTexture)
    : mRect(Rectangle{ pInitialPosition.x, pInitialPosition.y, 30, 30}), mRotation(pInitialRotation), mColor(pColor), mTexture(pTexture)
{
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

void Boid::Move(const std::vector<Boid*>& pBoids, const std::vector<Obstacle*>& pObstacles)
{
    Vector2 separateForce = Separate(pBoids);

    Vector2 separateScreen = AvoidScreenBorder();
    Vector2 separateObstacle = AvoidObstacles(pObstacles);
    Vector2 groupForce = Group(pBoids);
    Vector2 allignForce = Align(pBoids);

    mVelocity.x += (separateForce.x * 0.8) + (separateScreen.x * 0.2) + (separateObstacle.x * 0.2);
    mVelocity.y += (separateForce.y * 0.8) + (separateScreen.y * 0.2) + (separateObstacle.y * 0.2);

    mVelocity.x += groupForce.x;
    mVelocity.y += groupForce.y;

    mVelocity.x += allignForce.x;
    mVelocity.y += allignForce.y;

    Vector2 desiredVelocity = Normalize(mVelocity);

    float desiredAngle = atan2(desiredVelocity.y, desiredVelocity.x);

    float angleDiff = desiredAngle - mRotation;

    ClampAngle(angleDiff);

    float turnFactor = 0.05f;
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
            separation.x += diff.x;
            separation.y += diff.y;
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
    int Xmin = 100, Xmax = 980, Ymin = 100, Ymax = 620; // screen size - 100
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
    return Vector2{ 0.0f, 0.0f };
}

Vector2 Boid::Align(const std::vector<Boid*>& pOthers)
{
    Vector2 pV = { 0, 0 };
    for (const auto& boid : pOthers)
    {
        if (boid->GetID() == mId)
        {
            continue;
        }
        pV.x += boid->GetVelocity().x;
        pV.y += boid->GetVelocity().y;
    }
    pV.x /= pOthers.size() - 1;
    pV.y /= pOthers.size() - 1;

    return Vector2{ (pV.x - mVelocity.x) / 8, (pV.y - mVelocity.y) / 8 };
}

Vector2 Boid::Group(const std::vector<Boid*>& pOthers)
{
    Vector2 cM = { 0, 0 };
    for (const auto& boid : pOthers)
    {
        if (boid->GetID() == mId)
        {
            continue;
        }
        cM.x += boid->GetPosition().x;
        cM.y += boid->GetPosition().y;
    }
    cM.x /= pOthers.size() - 1;
    cM.y /= pOthers.size() - 1;

    return Vector2{(cM.x - mRect.x) / 100, (cM.y - mRect.y) / 100 };
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
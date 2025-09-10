#include "Boid.h"

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

void Boid::Move(std::vector<Boid*>& pBoids)
{
    if (mRect.x <= 0 || mRect.x >= 1080)
    {
        mVelocity.x = -mVelocity.x;
    }
    else if (mRect.y <= 0 || mRect.y >= 720)
    {
        mVelocity.y = -mVelocity.y;
    }

    mVelocity.x += Separate(pBoids).x;
    mVelocity.y += Separate(pBoids).y;

    Vector2 NormalizedVelocity = Normalize(mVelocity);

    Vector2 translation = { mMaxSpeed * NormalizedVelocity.x, mMaxSpeed * NormalizedVelocity.y };

    mRect.x += translation.x * GetFrameTime();
    mRect.y += translation.y * GetFrameTime();
}

Vector2 Boid::Separate(std::vector<Boid*>& pOthers)
{
    Vector2 AllDistances = Vector2{0, 0};
    for (auto boid : pOthers)
    {
        if (mId == boid->GetID())
        {
            continue;
        }

        Vector2 dist;
        dist.x = GetPosition().x - boid->GetPosition().x;
        dist.y = GetPosition().y - boid->GetPosition().y;

        float distanceSq = dist.x * dist.x + dist.y * dist.y; 

        if (distanceSq <= mMinimumDistance)
        {
            AllDistances.x += dist.x * 100;
            AllDistances.y += dist.y * 100;
        }
    }
    return Normalize(AllDistances);
}

Vector2 Boid::AvoidObstacles(std::vector<Obstacle*> pObstacles)
{
    return Vector2();
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
    DrawRectanglePro(mRect, origin, mRotation, WHITE);

    //DrawCircleLines(mRect.x, mRect.y, sqrt(mMinimumDistance), RED);
}

Vector2 Boid::Normalize(const Vector2& velocity)
{
    if (velocity.x == 0 || velocity.y == 0)
    {
        return velocity;
    }
    float newX = velocity.x / sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float newY = velocity.y / sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    return Vector2{ newX, newY };
}

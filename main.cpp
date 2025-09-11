#include "raylib.h"

#include "Boid.h"
#include "Obstacle.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void Update();
void Draw();
void InitSimulation();
void CleanUp();

std::vector<Obstacle*> obstacles;
std::vector<Boid*> boids;
std::vector<Boid*> boids1;
std::vector<Boid*> boids2;

int main() {
    cout << "Hello World" << endl;

    InitWindow(1920, 1080, "Boid Simulation");
    SetTargetFPS(60);

    InitSimulation();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CleanUp();
    CloseWindow();
    return 0;
}

void Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    for (const auto& obstacle : obstacles)
    {
        obstacle->Draw();
    }
    for (const auto& boid : boids)
    {
        boid->Draw();
    }
    for (const auto& boid : boids1)
    {
        boid->Draw();
    }
    for (const auto& boid : boids2)
    {
        boid->Draw();
    }
    EndDrawing();
}

void Update()
{
    for (auto& boid : boids)
    {
        boid->Move(boids, obstacles);
    }
    for (auto& boid : boids1)
    {
        boid->Move(boids1, obstacles);
    }
    for (auto& boid : boids2)
    {
        boid->Move(boids2, obstacles);
    }
}

void InitSimulation()
{
    Color color1 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    Color color2 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    Color color3 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    Texture tex = LoadTexture("resources/WhiteFish.png");
    for (int i = 0; i < 170; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(100, 980), (float)GetRandomValue(100, 620) }, GetRandomValue(0, 359), color1, tex);
        boids.push_back(boid);
    }
    for (int i = 0; i < 170; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(100, 980), (float)GetRandomValue(100, 620) }, GetRandomValue(0, 359), color2, tex);
        boids1.push_back(boid);
    }
    for (int i = 0; i < 170; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(100, 980), (float)GetRandomValue(100, 620) }, GetRandomValue(0, 359), color3, tex);
        boids2.push_back(boid);
    }
    /*Obstacle* obstacle = new Obstacle(Rectangle{ 540, 360, 200, 200 });
    obstacles.push_back(obstacle);*/
}

void CleanUp()
{
    for (auto& boid : boids)
    {
        delete boid;
    }
    boids.clear();
    for (auto& boid : boids1)
    {
        delete boid;
    }
    boids1.clear();
    for (auto& boid : boids2)
    {
        delete boid;
    }
    boids2.clear();
    for (auto& obstacle : obstacles)
    {
        delete obstacle;
    }
    obstacles.clear();
}
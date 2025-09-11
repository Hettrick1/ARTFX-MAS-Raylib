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

int main() {
    cout << "Hello World" << endl;

    InitWindow(1080, 720, "Boid Simulation");
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
    ClearBackground(DARKGRAY);
    for (const auto& obstacle : obstacles)
    {
        obstacle->Draw();
    }
    for (const auto& boid : boids)
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
}

void InitSimulation()
{

    for (int i = 0; i < 50; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(100, 980), (float)GetRandomValue(100, 620) }, GetRandomValue(0, 359));
        boids.push_back(boid);
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
    for (auto& obstacle : obstacles)
    {
        delete obstacle;
    }
    obstacles.clear();
}
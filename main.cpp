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

std::vector<std::vector<Boid*>> groupe12;
std::vector<std::vector<Boid*>> groupe13;
std::vector<std::vector<Boid*>> groupe23;

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
    for (const auto& obstacle : obstacles)
    {
        obstacle->Draw();
    }
    EndDrawing();
}

void Update()
{
    for (auto& boid : boids)
    {
        boid->Move(boids, groupe23,obstacles);
    }
    for (auto& boid : boids1)
    {
        boid->Move(boids1, groupe13,obstacles);
    }
    for (auto& boid : boids2)
    {
        boid->Move(boids2, groupe12,obstacles);
    }
}

void InitSimulation()
{
    Color color1 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    Color color2 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    Color color3 = { (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
    float size1 = 20;
    float size2 = 30;
    float size3 = 40;
    Texture tex = LoadTexture("resources/WhiteFish.png");
    for (int i = 0; i < 70; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(100, 400), (float)GetRandomValue(100, 200) }, size1, GetRandomValue(0, 359), color1, tex);
        boids.push_back(boid);
    }
    for (int i = 0; i < 20; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(600, 920), (float)GetRandomValue(300, 500) }, size2, GetRandomValue(0, 359), color2, tex);
        boids1.push_back(boid);
    }
    for (int i = 0; i < 20; i++)
    {
        Boid* boid = new Boid(Vector2{ (float)GetRandomValue(1200, 1820), (float)GetRandomValue(600, 800) }, size3, GetRandomValue(0, 359), color3, tex);
        boids2.push_back(boid);
    }

    groupe12.push_back(boids);
    groupe12.push_back(boids1);
    groupe23.push_back(boids1);
    groupe23.push_back(boids2);
    groupe13.push_back(boids);
    groupe13.push_back(boids2);

    Obstacle* obstacle = new Obstacle(Rectangle{ 260, 540, 200, 200 });
    obstacles.push_back(obstacle);
    Obstacle* obstacle1 = new Obstacle(Rectangle{ 1250, 540, 200, 300 });
    obstacles.push_back(obstacle1);
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
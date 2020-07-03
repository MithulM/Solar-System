#define OLC_PGE_APPLICATION
#include <iostream>
#include "C:\Users\mithu\Downloads\Mithul\Coding\C++\olcPixelGameEngine.h"
#include <ctime>
constexpr auto SCALE = 1;
constexpr auto PI = 3.141592653592653;
#define print(x) std::cout << x << std::endl;

double ran(double fMin, double fMax)
{
	return fMin + (double)rand() / RAND_MAX * (fMax - fMin);
}

class Ball
{
public:
	std::pair<double, double> pos;
	std::pair<double, double> vel;
	double radius;
	double coFric;
	int r;
	int g;
	int b;

	Ball(double x = 0, double y = 0, double vx = 0, double vy = 0, double radius = 1, double friction = 0, int r = 0, int g = 0, int b = 0)
		: pos({ x, y }), vel({vx, vy}), radius(radius), coFric(friction), r(r), g(g), b(b)
	{
	}
};

class BallPhysics : public olc::PixelGameEngine
{
private:
	std::vector<Ball> balls;
	int midX;
	int midY;
	double gravityX;
	double gravityY;
	bool bounce;
	int numBalls;

public:
	BallPhysics(int num) : numBalls(num)
	{
		sAppName = "Testing Graphics";
	}

	bool OnUserCreate() override
	{
		balls.reserve(150);

		midX = gravityX = ScreenWidth() / 2;
		midY = gravityY = ScreenHeight() / 2;

		bounce = true;

		/*
		for (int i = 0; i < numBalls; i++)
			balls.emplace_back(rand() % ScreenWidth(), rand() % ScreenHeight(), 0, 0, ran(5, 50), ran(1, 5), rand() % 255, rand() % 255, rand() % 255);
		*/

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		Clear(olc::BLACK);

		for (Ball& ball : balls)
		{

			ball.vel.first += ((gravityX - midX) - ball.coFric * ball.vel.first) * fElapsedTime;
			ball.vel.second += ((gravityY - midY) - ball.coFric * ball.vel.second) * fElapsedTime;

			ball.pos.first += ball.vel.first * fElapsedTime;
			ball.pos.second += ball.vel.second * fElapsedTime;

			FillCircle(olc::vi2d((int32_t)ball.pos.first, (int32_t)ball.pos.second), (int32_t)ball.radius, olc::Pixel(ball.r, ball.g, ball.b));
			DrawCircle(olc::vi2d((int32_t)ball.pos.first, (int32_t)ball.pos.second), (int32_t)ball.radius, olc::Pixel(255 - ball.r, 255 - ball.g, 255 - ball.b));
		}

		if (GetMouse(0).bPressed)
			balls.emplace_back(GetMouseX(), GetMouseY(), ran(-1000, 1000), ran(-1000, 1000), ran(5, 50), ran(.01, 5), rand() % 255, rand() % 255, rand() % 255);

		return true;
	}
};

int main()
{
	BallPhysics demo = BallPhysics(10);
	if (demo.Construct(1920, 1080, 1, 1, true, false))	demo.Start();
	return 0;
}

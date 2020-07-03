#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
#include <ctime>
constexpr auto SCALE = 1;
constexpr auto PI = 3.141592653592653;
#define print(x) std::cout << x << std::endl;

double ran(double fMin, double fMax)
{
	return fMin + (double)rand() / RAND_MAX * (fMax - fMin);
}

class Planet
{
public:
	std::pair<double, double> pos;
	std::pair<double, double> vel;
	double radius;
	std::vector<std::pair<int, int>> visited;
	int r;
	int g;
	int b;

	Planet(double x = 0, double y = 0, double vx = 0, double vy = 0, double radius = 1, int r = 0, int g = 0, int b = 0)
		: pos({ x, y }), vel({ vx, vy }), radius(radius), r(r), g(g), b(b)
	{
	}
};

class PlanetPhysics : public olc::PixelGameEngine
{
private:
	std::vector<Planet> planets;
	Planet sun;
	double coFric;
	int32_t startX;
	int32_t startY;
	int midX;
	int midY;

public:
	PlanetPhysics()
	{
		sAppName = "Testing Graphics";
	}

	bool OnUserCreate() override
	{
		planets.reserve(150);

		sun = { ScreenWidth() / 2.0, ScreenHeight() / 2.0, 0, 0, 10, 255, 255, 0 };

		midX = startX = ScreenWidth() / 2;
		midY = startY = ScreenHeight() / 2;

		return true;
	}

	void DrawArrow(double x1, double y1, double x2, double y2)
	{
		double theta = atan2(y2 - y1, x2 - x1);

		double Xside1 = x2 - 10 * sin(3 * PI / 4 - theta);
		double Yside1 = y2 - 10 * cos(3 * PI / 4 - theta);

		double Xside2 = x2 - 10 * sin(PI / 4 - theta);
		double Yside2 = y2 - 10 * cos(PI / 4 - theta);

		DrawLine(olc::vi2d({ (int32_t)x1, (int32_t)y1 }), olc::vi2d({ (int32_t)x2, (int32_t)y2 }));
		DrawLine(olc::vi2d({ ((int32_t)x2), (int32_t)y2 }), olc::vi2d({ (int32_t)Xside1, (int32_t)Yside1 }));
		DrawLine(olc::vi2d({ ((int32_t)x2), (int32_t)y2 }), olc::vi2d({ (int32_t)Xside2, (int32_t)Yside2 }));
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::BLACK);

		FillCircle(olc::vi2d((int32_t)sun.pos.first, (int32_t)sun.pos.second), (int32_t)sun.radius, olc::Pixel(sun.r, sun.g, sun.b));

		int32_t currX = GetMouseX();
		int32_t currY = GetMouseY();

		for (Planet& planet : planets)
		{
			double vectorX = sun.pos.first - planet.pos.first;
			double vectorY = sun.pos.second - planet.pos.second;
			double distance = sqrt(vectorX * vectorX + vectorY * vectorY);
			double gravity = sun.radius * planet.radius / distance / distance;

			std::pair<double, double> accel = { vectorX / distance * gravity, vectorY / distance * gravity };

			planet.vel.first += accel.first * fElapsedTime * 100000;
			planet.vel.second += accel.second * fElapsedTime * 100000;

			planet.pos.first += planet.vel.first * fElapsedTime;
			planet.pos.second += planet.vel.second * fElapsedTime;

			planet.visited.emplace_back(planet.pos);
			
			FillCircle(olc::vi2d((int32_t)planet.pos.first, (int32_t)planet.pos.second), (int32_t)planet.radius, olc::Pixel(planet.r, planet.g, planet.b));
			DrawCircle(olc::vi2d((int32_t)planet.pos.first, (int32_t)planet.pos.second), (int32_t)planet.radius, olc::Pixel(255 - planet.r, 255 - planet.g, 255 - planet.b));
			
			for (unsigned int i = 0; i < planet.visited.size(); i++)
				Draw(olc::vi2d(planet.visited[i].first, planet.visited[i].second), olc::Pixel(planet.r, planet.g, planet.b));

		}

		if (GetMouse(0).bPressed || GetMouse(1).bPressed || GetMouse(2).bPressed)
		{
			startX = currX;
			startY = currY;
		}

		if (GetMouse(0).bHeld || GetMouse(2).bHeld)
			DrawArrow(startX, startY, currX, currY);

		if (GetMouse(0).bReleased)
			planets.emplace_back(startX, startY, currX - startX, currY - startY, ran(1, 50), rand() % 255, rand() % 255, rand() % 255);

		if (GetMouse(2).bReleased)
		{
			double vectorX = sun.pos.first - startX;
			double vectorY = sun.pos.second - startY;
			double distance = sqrt(vectorX * vectorX + vectorY * vectorY);
			double radius = ran(5, 50);
			double s = log(sqrt(radius / distance * ((currX - startX) * (currX - startX) + (currY - startY) * (currY - startY)))) * 40;
			planets.emplace_back(startX, startY, vectorY / distance * s, -vectorX / distance * s, radius, rand() % 255, rand() % 255, rand() % 255);
		}

		if (GetMouse(1).bPressed)
		{
			double vectorX = sun.pos.first - startX;
			double vectorY = sun.pos.second - startY;
			double distance = sqrt(vectorX * vectorX + vectorY * vectorY);
			double radius = ran(5, 50);
			double s = sqrt(radius / distance) * 1000;
			planets.emplace_back(startX, startY, vectorY / distance * s, -vectorX / distance * s, radius, rand() % 255, rand() % 255, rand() % 255);
		}

		return true;
	}
};
int main()
{
	PlanetPhysics demo = PlanetPhysics();
	if (demo.Construct(1920, 1080, 1, 1, true, false))	demo.Start();
	return 0;
}

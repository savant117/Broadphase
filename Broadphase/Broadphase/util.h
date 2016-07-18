#pragma once

#include <math.h>

#define NOMINMAX
#include <Windows.h>

inline double timeMillis()
{
	unsigned __int64 freq, time;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER *)&time);
	return (double)time / freq * 1000.0;
}

inline float randf()
{
	return rand() / (float)RAND_MAX;
}

inline float randf(float from, float to)
{
	return randf() * (from - to) + from;
}

inline float min(float a, float b)
{
	return a < b ? a : b;
}

inline float max(float a, float b)
{
	return a > b ? a : b;
}

struct float2
{
	float x;
	float y;

	float2()
	{}

	float2(float x_, float y_)
		: x(x_), y(y_)
	{}
};

struct aabb2
{
	float2 min;
	float2 max;

	struct aabb2()
	{}

	struct aabb2(const float2& min_, const float2& max_)
		: min(min_), max(max_)
	{}

	bool intersect(const aabb2& box) const
	{
		return !(min.x > box.max.x
			|| max.x < box.min.x
			|| min.y > box.max.y
			|| max.y < box.min.y);
	}
};

struct ray2
{
	float2 start;
	float2 direction;

	struct ray2()
	{}

	struct ray2(const float2& start_, const float2& direction_)
		: start(start_), direction(direction_)
	{}

	bool intersect(const aabb2& box) const
	{
		float tmin = -FLT_MAX, tmax = FLT_MAX;

		if (direction.x != 0) 
		{
			float tx1 = (box.min.x - start.x) / direction.x;
			float tx2 = (box.max.x - start.x) / direction.x;

			tmin = max(tmin, min(tx1, tx2));
			tmax = min(tmax, max(tx1, tx2));
		}

		if (direction.y != 0) 
		{
			float ty1 = (box.min.y - start.y) / direction.y;
			float ty2 = (box.max.y - start.y) / direction.y;

			tmin = max(tmin, min(ty1, ty2));
			tmax = min(tmax, max(ty1, ty2));
		}

		return tmax >= tmin;
	}
};
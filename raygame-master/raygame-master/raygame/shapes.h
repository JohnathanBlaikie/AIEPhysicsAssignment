#pragma once

#include <cstdint>

struct circle
{
	float radius;
};

enum class shapeType : uint8_t
{
	None = 0,
	Circle = 1 << 0,
	AABB = 1<<1

};

struct collider
{
	shapeType colliderShape;
	union {
		circle circleData;
	};
};

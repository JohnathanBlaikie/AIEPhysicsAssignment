#pragma once

#include <cstdint>
#include "glm/vec2.hpp"

struct circle
{
	float radius;
};

struct AABB
{
	float length;
	float width;

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

		AABB boxData;
	};
};

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB);
bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB);

bool checkAABB2(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB);
bool checkAABB2(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB);

bool checkAABBCircle(glm::vec2 boxPos, AABB box, glm::vec2 circlePos, circle circle );
bool checkAABBCircle(glm::vec2 boxPos, collider box, glm::vec2 circlePos, collider circle );
//vec2 for collision normal
//pos of shape A
//shape of shape A
//pos of shape B
//shape of shape B
//penetration distance
glm::vec2 depenetrateCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen);
glm::vec2 depenetrateCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float& pen);

glm::vec2 depenetrateAABB2(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB, float& pen);
glm::vec2 depenetrateAABB2(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB, float& pen);

glm::vec2 depenetrateAABBCircle(glm::vec2 boxPos, AABB box, glm::vec2 circlePos, circle circle, float& pen);
glm::vec2 depenetrateAABBCircle(glm::vec2 boxPos, collider box, glm::vec2 circlePos, collider circle, float& pen);

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA,
	glm::vec2 posB, glm::vec2 velB, float massB,
	float elasticity, glm::vec2 colNormal, glm::vec2 *dst);


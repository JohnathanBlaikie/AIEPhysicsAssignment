#include "shapes.h"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <cmath>

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB)
{
	//glm::vec2 offset = (posA - posB);
	//float distance = (glm::dot(offset, offset));
	//
	//float sum = (circleA.radius + circleA.radius);
	//
	//return distance < (sum * sum);

	float distanceSq = glm::length2(posA - posB);

	float sum = circleA.radius + circleB.radius;

	return distanceSq < sum* sum;
}

bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB)
{
	return checkCircleCircle(posA, circleA.circleData, posB, circleB.circleData);
}

bool checkAABB2(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB)
{
	////calculate distance
	//float dist = glm::length(posA - posB);

	bool overlap = (posA.x + boxA.length > posB.x && posA.x < posB.x + boxB.length)
		&& (posA.y + boxA.width > posB.y && posA.y < posB.y + boxB.width);

	//return collision
	return overlap;
}

bool checkAABB2(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB)
{
	return checkAABB2(posA, boxA.boxData, posB, boxB.boxData);
}

bool checkAABBCircle(glm::vec2 boxPos, AABB box, glm::vec2 posA, circle circleA) 
{
	return false;
}
bool checkAABBCircle(glm::vec2 boxPos, collider box, glm::vec2 circlePos, collider circle) 
{
	return false;
}

glm::vec2 depenetrateCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen)
{
	//calculate distance
	float dist = glm::length(posA - posB);
	float sum = circleA.radius + circleB.radius;
	//pen is equal to the difference between the sum of their radii sub distance between the two circles
	pen = sum - dist;
	//collision normal is the normalized vector between their two positions
	return glm::normalize(posA - posB);

}

glm::vec2 depenetrateCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float& pen)
{
	return depenetrateCircleCircle(posA, circleA.circleData, posB, circleB.circleData, pen);
}

glm::vec2 depenetrateAABB2(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB, float& pen)
{
	//this gets the center
	glm::vec2 boxACenter = glm::vec2{ (posA.x + (boxA.length * 0.5f)),(posA.y + (boxA.width * 0.5f)) };
	glm::vec2 boxBCenter = glm::vec2{ (posB.x + (boxB.length * 0.5f)),(posB.y + (boxB.width * 0.5f)) };

	//this finds the offset
	glm::vec2 offset = glm::vec2{abs((boxACenter.x - boxBCenter.x)), abs((boxACenter.y - boxBCenter.y))};

	//this finds the difference
	float xPen = abs((boxACenter.x + (boxA.length * 0.5f)) - (boxBCenter.x + (boxB.length * 0.5f)));
	float yPen = abs((boxACenter.y + (boxA.width * 0.5f)) - (boxBCenter.y + (boxB.width * 0.5f)));

	xPen -= offset.x;
	yPen -= offset.y;

	if (xPen > yPen)
	{
		pen = xPen;
	}
	else
	{
		pen = yPen;
	}

	//return glm::normalize(posA - posB);

	return glm::normalize(boxACenter - boxBCenter);
	
}

glm::vec2 depenetrateAABB2(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB, float& pen)
{
	return depenetrateAABB2(posA, boxA.boxData, posB, boxB.boxData, pen);
}

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA,
					  glm::vec2 posB, glm::vec2 velB, float massB,
					  float elasticity, glm::vec2 colNormal, glm::vec2* dst) 
{
	//calculate relative velocity
	glm::vec2 relVel = velA - velB;
	//calculate impulse magnitude
	float impulseMag = glm::dot(-(1.0f + elasticity) * relVel, colNormal) /
					   glm::dot(colNormal, colNormal * (1 / massA + 1 / massB));
	//write out impulses for the two objects into the given array
	dst[0] = velA + (impulseMag / massA) * colNormal;
	dst[1] = velB - (impulseMag / massB) * colNormal;

}


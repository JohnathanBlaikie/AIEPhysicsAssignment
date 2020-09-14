#include "shapes.h"
#include "glm/glm.hpp"
#include <cmath>

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB)
{
	glm::vec2 offset = (posA - posB);
	float distance = (glm::dot(offset, offset));

	float sum = (circleA.radius + circleA.radius);

	return distance < (sum * sum);
}

bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB)
{
	return checkCircleCircle(posA, circleA.circleData, posB, circleB.circleData);
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

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA,
					  glm::vec2 posB, glm::vec2 velB, float massB,
					  float elasticity, glm::vec2 colNormal, glm::vec2* dst) 
{
	//calculate relative velocity
	glm::vec2 relVel = velA - velB;
	//calculate impulse magnitude
	float impulseMag = glm::dot(-(-1.0f + elasticity) * relVel, colNormal) /
					   glm::dot(colNormal, colNormal * (1 / massA + 1 / massB));
	//write out impulses for the two objects into the given array
	dst[0] = velA + (impulseMag / massA) * colNormal;
	dst[1] = velB - (impulseMag / massB) * colNormal;

}


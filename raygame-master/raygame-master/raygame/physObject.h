#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "raylib.h"
#include "shapes.h"


class physObject
{
	//forces
	glm::vec2 forces;

public:
	//position
	glm::vec2 pos;
	//velocity
	glm::vec2 vel;
	//mass
	float mass;
	
	collider shape;

	//constructor (defaults)
	physObject();
	//tickPhysics
	void tickPhysics(float deltaTime);
	//draw
	void draw() const;
	//addForce
	void addForce(glm::vec2 force);
	//addImpulse
	void addImpulse(glm::vec2 impulse);
	//addAcceleration
	void addAccel(glm::vec2 accel);
	//addVelocityChange
	void addVelocityChange(glm::vec2 velChng);
	//addDrag
	void addDrag(glm::vec2 velo);

	// Sets a point A and B, then creates a line between the two
	void makeLine(Vector2 v2Start, Vector2 v2End);

	// Sets direction and force based on the difference in position of point A and B
	void setDirection(glm::vec2 start, glm::vec2 end);
};


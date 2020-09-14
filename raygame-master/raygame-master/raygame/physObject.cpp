#include "physObject.h"
#include <vector>
#include "raylib.h"
physObject::physObject()
{
	pos = glm::vec2{ 0,0 };
	vel = glm::vec2{ 0,0 };
	forces = glm::vec2{ 0,0 };

	mass = 1.0f;
	shape = { shapeType::Circle, circle{10.0f} };
}

void physObject::tickPhysics(float deltaTime)
{
	//integrate forces into velocity
	vel += forces * deltaTime;
	forces = { 0,0 };
	//TODO: Implement some linear drag

	//Integrate velocity into position
	pos += vel * deltaTime;
}

void physObject::draw() const
{
	switch (shape.colliderShape)
	{
	case shapeType::None:
		DrawPixel((int)pos.x, (int)pos.y, RED);
	case shapeType::Circle:
		DrawCircleLines((int)pos.x, (int)pos.y, shape.circleData.radius, RED);
	//case shapeType::AABB:
	//	assert(false && "AABB not yet implemented");
	default:
		break;
	}

	//DrawCircle((int)pos.x, (int)pos.y, 10, RED);
	//DrawPixel((int)pos.x, (int)pos.y, RED);
}
void physObject::addForce(glm::vec2 force)
{
	forces += force / mass;
}
void physObject::addImpulse(glm::vec2 impulse)
{
	vel += impulse / mass;

}
void physObject::addDrag(glm::vec2 velo)
{
	//vel -= mass / velo;
	vel -= velo / mass;
}
void physObject::makeLine(Vector2 v2Start, Vector2 v2End)
{
	DrawLine(v2Start.x, v2Start.y, v2End.x, v2End.y, RED);
}

void physObject::setDirection(glm::vec2 v2Start, glm::vec2 v2End)
{
	
	
}

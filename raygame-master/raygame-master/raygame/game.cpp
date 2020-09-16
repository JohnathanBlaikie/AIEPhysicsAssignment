#include "game.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include "raylib.h"
#include "enumUtils.h"

collisionMap setupCollisionChecks()
{
	collisionMap map;

	map[static_cast<collisionPair>(shapeType::Circle | shapeType::Circle)] = checkCircleCircle;
	
	map[static_cast<collisionPair>(shapeType::AABB | shapeType::AABB)] = checkAABB2;

	map[static_cast<collisionPair>(shapeType::AABB | shapeType::Circle)] = checkAABBCircle;


	return map;

}

depenetrationMap setupDepenetrationFuncs()
{
	depenetrationMap map;

	map[(collisionPair)(shapeType::Circle | shapeType::Circle)] = depenetrateCircleCircle;
	map[(collisionPair)(shapeType::AABB | shapeType::AABB)] = depenetrateAABB2;
	//map[(collisionPair)(shapeType::AABB | shapeType::Circle)] = depenetrateAABBCircle;

	//TODO: ADD CIRCLE-AABB depen
	//TODO: ADD AABB-AABB depen

	return map;
}

collisionMap game::collisionCheckers = setupCollisionChecks();
depenetrationMap game::depenetrationFuncs = setupDepenetrationFuncs();
// RETURN_TYPE(*)(PARAMETER_TYPES)

// inclusion order
// - header that I am implementing
// - system libraries
// - project libraries
// - other headers from this project

game::game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
	srand(time(0));
}

void game::init()
{
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
	
	isMouseClicked = false;
}

bool game::tick()
{
	accumulatedDeltaTime += GetFrameTime(); // aka add delta time to accmulator
	
	
	if (IsMouseButtonPressed(0))
	{
		cursorPosStart = GetMousePosition();
		isMouseClicked = true;
	}
	else if (IsMouseButtonPressed(1))
	{
		cursorPosStart = GetMousePosition();
		isMouseClicked = true;
	}
	if (isMouseClicked)
	{
		physObject line;
		cursorPosEnd = GetMousePosition();
		line.makeLine(cursorPosStart, cursorPosEnd);
	}
	if (IsMouseButtonReleased(0))
	{
		isMouseClicked = false;
		physObject baby;
		baby.shape = { shapeType::Circle };
		baby.pos = { cursorPosEnd.x, cursorPosEnd.y };
		baby.mass = (rand() % 20 + 5);
		//baby.mass = (rand() % 10) + 1;
		//baby.mass = 10;
		baby.shape.circleData.radius = baby.mass;
		baby.addImpulse({ (cursorPosStart.x - cursorPosEnd.x)*10,(cursorPosStart.y - cursorPosEnd.y)*10 });
		physObjects.push_back(baby);
	}
	else if (IsMouseButtonReleased(1))
	{
		isMouseClicked = false;
		physObject baby;
		baby.shape = { shapeType::AABB };
		baby.pos = { cursorPosEnd.x, cursorPosEnd.y };
		baby.mass = (rand() % 20 + 5);
		//baby.mass = (rand() % 10) + 1;
		//baby.mass = 10;
		baby.shape.boxData.width = baby.mass;
		baby.shape.boxData.length = baby.mass;
		baby.addImpulse({ (cursorPosStart.x - cursorPosEnd.x) * 10,(cursorPosStart.y - cursorPosEnd.y) * 10 });
		physObjects.push_back(baby);
	}

	//VVVV This stuff works and spawns a sphere. VVVV
	//if (IsMouseButtonPressed(0))
	//{
	//	auto cursorPos = GetMousePosition();
	//
	//	physObject baby;
	//	baby.pos = { cursorPos.x,cursorPos.y };
	//	baby.mass = (rand() % 10) + 1;
	//	baby.addImpulse({ cursorPos.x,cursorPos.y });
	//	physObjects.push_back(baby);
	//
	//}
	if (IsMouseButtonPressed(2))
	{
		auto cursorPos = GetMousePosition();
	
		physObject baby;
		baby.shape = {shapeType::AABB};
		baby.pos = { cursorPos.x,cursorPos.y };
		//baby.mass = (rand() % 10) + 5;
		baby.mass = 100;
		baby.shape.boxData.length = baby.mass;
		baby.shape.boxData.width = baby.mass;
		//baby.addImpulse({ cursorPos.x,cursorPos.y });
		physObjects.push_back(baby);
	
	}
	std::cout << "\n" << std::endl;

	return !WindowShouldClose();
}

void game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	for (auto& lhs : physObjects)
	{
		for (auto& rhs : physObjects)
		{
			if (&lhs == &rhs) { continue; }

			auto *first = &lhs;
			auto *second = &rhs;

			if (static_cast<uint8_t>(lhs.shape.colliderShape) >
				static_cast<uint8_t>(rhs.shape.colliderShape))
			{
				first = &rhs;
				second = &lhs;
			}

			collisionPair pairType = (collisionPair)(lhs.shape.colliderShape | rhs.shape.colliderShape);

			if (collisionCheckers.find(pairType) == collisionCheckers.end())
			{
				continue;
			}
			//collisionCheckers<int, shapeType> test = { {1, shapeType::AABB},{2, shapeType::AABB} };
			//auto search = test.find(2);
			//if (search != test.end()) {
			//	std::cout << "Found " << search->first << " " << search->second << '\n';
			//}
			//else {
			//	std::cout << "Not found\n";
			//}

			bool collision = collisionCheckers[pairType](first->pos, first->shape, second->pos, second->shape);


			if (collision)
			{
				std::cout << "Collision!\n";
				float pen = 0.0f;

				glm::vec2 normal = depenetrationFuncs[pairType](first->pos, first->shape,
																second->pos, second->shape,
																pen);

				glm::vec2 resImpulses[2];
				resolveCollision(first->pos, first->vel, first->mass,
								 second->pos, second->vel, second->mass,
								 1.0f, normal, resImpulses);
				pen *= .51f;

				first->pos += normal * pen;
				second->pos -= normal * pen;

				first->vel = resImpulses[0];
				second->vel = resImpulses[1];

			}

			//if (checkCircleCircle(lhs.pos, lhs.shape.circleData, rhs.pos, rhs.shape.circleData)) {
			//	std::cout << "Collision!\n";
			//}
		}
	}

	for (auto& obj : physObjects)
	{
		obj.tickPhysics(fixedTimeStep);
		//obj.addDrag(obj.vel);
	}

	// do physics!
	//std::cout << "do physics" << std::endl;
}

bool game::shouldTickPhysics() const
{
	return accumulatedDeltaTime >= fixedTimeStep;
}

void game::screenWrap()
{

}

void game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto& obj : physObjects)
	{
		obj.draw();
	}

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}

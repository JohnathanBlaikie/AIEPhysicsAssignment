#include "game.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>


#include "raylib.h"


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
		baby.pos = { cursorPosEnd.x, cursorPosEnd.y };
		baby.mass = (rand() % 10) + 1;
		baby.addImpulse({ (cursorPosStart.x - cursorPosEnd.x)*10,(cursorPosStart.y - cursorPosEnd.y)*10 });
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

	std::cout << "do update" << std::endl;

	return !WindowShouldClose();
}

void game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	for (auto& obj : physObjects)
	{
		obj.tickPhysics(fixedTimeStep);
		obj.addDrag(obj.vel);
	}

	// do physics!
	std::cout << "do physics" << std::endl;
}

bool game::shouldTickPhysics() const
{
	return accumulatedDeltaTime >= fixedTimeStep;
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

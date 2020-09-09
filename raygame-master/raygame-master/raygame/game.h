#pragma once
#include <vector>
#include "physObject.h"

class game
{
	float accumulatedDeltaTime;
	std::vector<physObject> physObjects;
public:
	game();

	bool isMouseClicked;
	Vector2 cursorPosStart = Vector2{ 0,0 };
	Vector2 cursorPosEnd = Vector2{ 0,0 };
	// Time between every fixed update
	float fixedTimeStep;

	// Create our window and initialize our rendering context
	void init();

	// Update the logic for our game
	// - Return true as long as window should stay open
	bool tick();

	// Update the physics logic for our game
	// - this should only run every fixed timespan
	void tickPhysics();

	// Return true when a physics update is ready
	bool shouldTickPhysics() const;

	// Render all of the objects for our game
	void draw() const;

	// Destroy our window and deinitialize our rendering context
	void exit();
};


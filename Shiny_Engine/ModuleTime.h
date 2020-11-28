#pragma once
#include "Module.h"
#include "Timer.h"

enum class GameState {
	EDITOR = 0,
	PLAYING,
	PAUSE
};

static class Time : public Module
{
public:
	Time(Application* app, bool start_enabled = true);
	~Time();

	bool Start();

	update_status PreUpdate();

public:

	static float dt;

	Timer timer;

	static GameState gameState;

	float timeScale = 1.0f;
};
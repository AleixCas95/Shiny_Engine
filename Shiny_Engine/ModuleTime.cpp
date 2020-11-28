#include "ModuleTime.h"

float Time::dt = 0.0f;

GameState Time::gameState = GameState::EDITOR;

Time::Time(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Time::~Time()
{
}

bool Time::Start()
{
	timer.Start();

	return true;
}

update_status Time::PreUpdate()
{
	float realDT = (float)timer.Read() / 1000.f;

	timer.Start();

	switch (gameState)
	{
	case GameState::EDITOR:
		dt = realDT;
		break;
	case GameState::PLAYING:
		dt = realDT * timeScale;
		break;
	case GameState::PAUSE:
		dt = 0.0f;
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}
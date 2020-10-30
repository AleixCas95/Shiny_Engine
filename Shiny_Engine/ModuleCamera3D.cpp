#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleConsole.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	CalculateViewMatrix();

	X = math::float3(1.0f, 0.0f, 0.0f);
	Y = math::float3(0.0f, 1.0f, 0.0f);
	Z = math::float3(0.0f, 0.0f, 1.0f);

	Position = math::float3(0.0f, 3.0f, 3.0f);
	Reference = math::float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}


bool ModuleCamera3D::Start()
{
	App->console->AddLog("Setting up the camera");
	bool ret = true;

	return ret;
}

bool ModuleCamera3D::CleanUp()
{
	App->console->AddLog("Cleaning camera");

	return true;
}



update_status ModuleCamera3D::Update(float dt)
{
	math::float3 newPos(0, 0, 0);
	float speed = camera_mov_speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = camera_mov_speed * 2 * dt;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += Y * speed;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos -= Y * speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	
	if (App->input->GetMouseZ() != 0)
	{
		newPos -= App->input->GetMouseZ() * Z * mouse_wheel_speed;
	}

	Position += newPos;
	Reference += newPos;

	// Mouse motion
	//Mouse centered Rotation

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
	
		
			// Look Around (target position)
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float deltaX = (float)dx * dt;
			float deltaY = (float)dy * dt;
		
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * mouse_sensitivity * dt;

			X = math::float3x3::RotateY(DeltaX) * X;
			Y = math::float3x3::RotateY(DeltaX) * Y;
			Z = math::float3x3::RotateY(DeltaX) * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * mouse_sensitivity * dt;

			Y = math::float3x3::RotateAxisAngle(X, DeltaY) * Y;
			Z = math::float3x3::RotateAxisAngle(X, DeltaY) * Z;

			if (Y.y < 0.0f)
			{
				Z = math::float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = math::Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Look(const math::float3& Position, const math::float3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt(const math::float3& Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::Move(const math::float3& Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::Orbit(math::float3 target, float deltaX, float deltaY)
{
	Position -= target;

	if (deltaY != 0.0f)
	{
		math::float3x3 rotationMatrix = math::float3x3::RotateY(deltaY);
		X = rotationMatrix * X;
		Y = rotationMatrix * Y;
		Z = rotationMatrix * Z;
	}

	if (deltaX != 0.0f)
	{
		math::float3x3 rotationMatrix = math::float3x3::RotateAxisAngle(X, deltaX);
		Y = rotationMatrix * Y;
		Z = rotationMatrix * Z;
	}

	Position = target + (Z * Position.Length());

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.ptr();
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

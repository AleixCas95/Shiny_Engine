#ifndef MODULERENDERER3D_H
#define MODULERENDERER3D_H


#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleFBX.h"
#include "ComponentCamera.h"

#define MAX_LIGHTS 8
class Application;
class Mesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool DrawMeshes(const ModelConfig mesh) const;
	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	std::vector<ModelConfig> meshes;


	std::vector<Mesh*> mesh_list;

	
	float tex_alpha = 0.0f;

	//Camera

	ComponentCamera* current_cam = nullptr;
	ComponentCamera* play_cam = nullptr;
};

#endif // !MODULERENDERER3D_H
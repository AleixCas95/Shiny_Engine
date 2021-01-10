#include "ModuleShapes.h"
#include "Application.h"
#include "ParShapes/par_shapes.h"
#include "ModuleGUI.h"

ModuleShapes::ModuleShapes(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}
ModuleShapes::~ModuleShapes()
{
}


bool ModuleShapes::Start()
{
	return true;
}
update_status ModuleShapes::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleShapes::CleanUp()
{
	return true;
}

void ModuleShapes::Draw()

{

	if (ImGui::Button("Sphere"))
	{
		par_shapes_mesh* shape = par_shapes_create_subdivided_sphere(3);
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Sphere", uuid);
		par_shapes_free_mesh(shape);

	}

	ImGui::Separator();

	if (ImGui::Button("Cube"))
	{
		par_shapes_mesh* shape = par_shapes_create_cube();
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Cube", uuid);
		par_shapes_free_mesh(shape);
	}

	ImGui::Separator();

	if (ImGui::Button("Cylinder"))
	{
		par_shapes_mesh* shape = par_shapes_create_cylinder(15, 15);
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Cylinder", uuid);
		par_shapes_free_mesh(shape);
	}

	ImGui::Separator();

	if (ImGui::Button("Cone"))
	{
		par_shapes_mesh* shape = par_shapes_create_cone(15, 15);
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Cone", uuid);
		par_shapes_free_mesh(shape);
	}

	ImGui::Separator();

	if (ImGui::Button("Knot"))
	{
		par_shapes_mesh* shape = par_shapes_create_trefoil_knot(15, 15, 2.0f);
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Knot", uuid);
		par_shapes_free_mesh(shape);
	}

	ImGui::Separator();

	if (ImGui::Button("Torus"))
	{
		par_shapes_mesh* shape = par_shapes_create_torus(15, 15, 0.3f);
		ResourceMesh* mesh = App->fbx->MeshParShape(shape, "Torus", uuid);
		par_shapes_free_mesh(shape);
	}

}

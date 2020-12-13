#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include "Primitive.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */



ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}
// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->console->AddLog("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		App->console->AddLog("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		LOG("Glew Error: ", glewGetErrorString(err));
	}
	else
	{
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->console->AddLog("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->console->AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	if (Time::gameState != GameState::EDITOR)
	{
		current_cam->UpdateFrustum();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glLoadMatrixf(current_cam->GetProjectionMatrix().ptr());


	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(current_cam->GetProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(current_cam->GetViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(current_cam->frustum.pos.x, current_cam->frustum.pos.y, current_cam->frustum.pos.z);
	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	for (std::list<ComponentMesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); ++it)
	{
		(*it)->Draw();
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (culling && play_cam)
	{
		std::list<ComponentMesh*> toDraw;

		for (std::list<ComponentMesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); ++it)
		{
			if (play_cam->frustum.Intersects((*it)->gameObject->boundingBox))
			{
				toDraw.push_back(*it);
			}
		}

		for (std::list<ComponentMesh*>::iterator it = toDraw.begin(); it != toDraw.end(); ++it)
		{
			(*it)->Draw();
		}
		toDraw.clear();
	}

	//Debug Draw
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		drawBoxes = !drawBoxes;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		DebugTextures();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		culling = !culling;
	}

	bool wireframeMode = false;
	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);

	if (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE)
		wireframeMode = true;

	bool cullFace = glIsEnabled(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	if (drawBoxes)
	{
		for (auto gameobject : App->gobject->gameObjects)
		{
			float3 corners[8];
			gameobject->boundingBox.GetCornerPoints(corners);

			glLineWidth(2.0f);

			if (gameobject == App->scene->current_object)
				glColor3f(0.0f, 1.0f, 0.0f);

			else
				glColor3f(0.0f, 0.0f, 1.0f);

			glVertex3fv((GLfloat*)&corners[1]);
			glVertex3fv((GLfloat*)&corners[5]);
			glVertex3fv((GLfloat*)&corners[7]);
			glVertex3fv((GLfloat*)&corners[3]);

			glVertex3fv((GLfloat*)&corners[4]);
			glVertex3fv((GLfloat*)&corners[0]);
			glVertex3fv((GLfloat*)&corners[2]);
			glVertex3fv((GLfloat*)&corners[6]);

			glVertex3fv((GLfloat*)&corners[5]);
			glVertex3fv((GLfloat*)&corners[4]);
			glVertex3fv((GLfloat*)&corners[6]);
			glVertex3fv((GLfloat*)&corners[7]);

			glVertex3fv((GLfloat*)&corners[0]);
			glVertex3fv((GLfloat*)&corners[1]);
			glVertex3fv((GLfloat*)&corners[3]);
			glVertex3fv((GLfloat*)&corners[2]);

			glVertex3fv((GLfloat*)&corners[3]);
			glVertex3fv((GLfloat*)&corners[7]);
			glVertex3fv((GLfloat*)&corners[6]);
			glVertex3fv((GLfloat*)&corners[2]);

			glVertex3fv((GLfloat*)&corners[0]);
			glVertex3fv((GLfloat*)&corners[4]);
			glVertex3fv((GLfloat*)&corners[5]);
			glVertex3fv((GLfloat*)&corners[1]);

			glEnd();
		}

		std::vector<math::AABB> vecquad;

		glLineWidth(2.0f);

		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_QUADS);

		
		glEnd();
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->console->AddLog("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}
bool ModuleRenderer3D::DrawMeshes(const ModelConfig mesh) const
{
	bool ret = true;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// --- Texture ---
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);



	if (App->fbx->last_texture_id == 0)
	{
		// Alpha
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, tex_alpha);
		// -- end alpha
		glBindTexture(GL_TEXTURE_2D, mesh.texture_id);
	}

	else
	{
		// Alpha
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, tex_alpha);
		// -- end alpha
		glBindTexture(GL_TEXTURE_2D, App->fbx->last_texture_id);
	}


	// --- End texture ---

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	return ret;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DebugTextures()
{
	for (auto gameobject : App->gobject->gameObjects)
	{
		if (gameobject->HasComponent(Object_Type::CompTexture))
		{
			ComponentTexture* tex = (ComponentTexture*)gameobject->GetComponent(Object_Type::CompTexture);
			if (paintTextures)
			{
				tex->print = false;
			}
			else if (!paintTextures)
			{
				tex->print = true;
			}
		}
	}
	paintTextures = !paintTextures;
}
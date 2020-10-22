#include "ModuleGeometry.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleGeometry::~ModuleGeometry()
{
}


bool ModuleGeometry::Start() {

	return ret;
}

void ModuleGeometry::Draw() {
	glBegin(GL_TRIANGLES);


	glClearColor(1, 1, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(2, 0, 0);


	//// Right face
	GLfloat vertex[] =

		//front
	{ 5.0f,1.0f,0.0f,
		4.0f,1.0f,0.0f,
		4.0f,0.0f,0.0f,

		//front2

		4.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 0.0f,
		5.0f, 1.0f, 0.0f,

		//right
		5.0f, 0.0f, 0.0f,
		5.0f, 0.0f, -1.0f,
		5.0f, 1.0f, -1.0f,

		//right2
		5.0f, 0.0f, 0.0f,
		5.0f, 1.0f,-1.0f,
		5.0f, 1.0f, 0.0f,

		//back
		5.0f, 0.0f, -1.0f,
		4.0f, 1.0f, -1.0f,
		5.0f, 1.0f, -1.0f,


		//back2
		5.0f, 0.0f, -1.0f,
		4.0f, 0.0f, -1.0f,
		4.0f, 1.0f, -1.0f,



		//top
		4.0f, 1.0f, 0.0f,
		5.0f, 1.0f, 0.0f,
		5.0f, 1.0f, -1.0f,

		//top2
		4.0f, 1.0f, 0.0f,
		5.0f, 1.0f, -1.0f,
		4.0f, 1.0f, -1.0f,

		//left
		4.0f, 0.0f, 0.0f,
		4.0f, 1.0f, 0.0f,
		4.0f, 1.0f, -1.0f,

		//left2
		4.0f, 0.0f, 0.0f,
		4.0f, 1.0f, -1.0f,
		4.0f, 0.0f, -1.0f,

		//bottom
		4.0f, 0.0f, 0.0f,
		4.0f, 0.0f, -1.0f,
		5.0f, 0.0f, -1.0f,

		//bottom2
		4.0f, 0.0f, 0.0f,
		5.0f, 0.0f, -1.0f,
		5.0f, 0.0f, 0.0f,


	};

	glColor3f(4, 0, 0);	// Front face
	glTexCoord2f(0, 0); glVertex3f(-1, 1, 1); // v1
	glTexCoord2f(0, 1); glVertex3f(-1, -1, 1); // v2
	glTexCoord2f(1, 1); glVertex3f(1, -1, 1); // v3
	glTexCoord2f(0, 0); glVertex3f(-1, 1, 1); // v1
	glTexCoord2f(0, 1); glVertex3f(1, -1, 1); // v2
	glTexCoord2f(1, 1); glVertex3f(1, 1, 1); // v0
	// Left face
	glTexCoord2f(1, 1); glVertex3f(1, 1, 1); // v0
	glTexCoord2f(0, 1); glVertex3f(1, -1, 1); // v3
	glTexCoord2f(1, 1); glVertex3f(1, -1, -1); // v4
	glTexCoord2f(0, 0); glVertex3f(1, 1, 1); // v0
	glTexCoord2f(1, 1); glVertex3f(1, -1, -1); // v4
	glTexCoord2f(1, 0); glVertex3f(1, 1, -1); // v5
	//// Back face
	glTexCoord2f(0, 0); glVertex3f(-1, -1, -1); // v7
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1); // v5
	glTexCoord2f(0, 1); glVertex3f(1, 1, -1); // v6
	glTexCoord2f(0, 0); glVertex3f(-1, -1, -1); // v7
	glTexCoord2f(1, 0); glVertex3f(1, 1, -1); // v4
	glTexCoord2f(1, 1); glVertex3f(1, -1, -1); // v5
		// right face
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 1); //v0
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1); // v3
	glTexCoord2f(1, 0); glVertex3f(-1, 1, -1); // v5
	glTexCoord2f(0, 1); glVertex3f(-1, -1, 1); // v3
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1); // v4
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1); // v5
		//Top face
	glTexCoord2f(1, 1); glVertex3f(1, 1, 1); // v0
	glTexCoord2f(1, 0); glVertex3f(1, 1, -1); // v5
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1); // v6
	glTexCoord2f(1, 1); glVertex3f(1, 1, 1); // v0
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1); // v6
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1); // v1
		//bottom face
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1); // v3
	glTexCoord2f(1, 1); glVertex3f(1, -1, 1); // v4
	glTexCoord2f(0, 1); glVertex3f(-1, -1, 1); // v7
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1); // v3
	glTexCoord2f(0, 1); glVertex3f(-1, -1, 1); // v7
	glTexCoord2f(0, 0); glVertex3f(-1, -1, -1); // v2
	glEnd();

	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertex, GL_STATIC_DRAW);

}

update_status ModuleGeometry::Update(float dt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	return UPDATE_CONTINUE;
}
#include "Primitive.h"
#include "Glew/include/glew.h"
#include "Application.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

void Mesh::InnerRender() const
{
	ComponentTransform* transform = parent->transform;
	glPushMatrix();
	float4x4 mat = transform->GetMatrixOGL();

	glMultMatrixf(mat.ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertex.id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.id);

	if (parent)
	{
		ComponentTexture* tex = (ComponentTexture*)parent->GetComponent(CompTexture);
		if (tex)
		{
			if (tex->print)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tex->GetID());
			}
		}

		ComponentMesh* mesh = (ComponentMesh*)parent->GetComponent(CompMesh);

		if (mesh && mesh->printVertexNormals && hasNormals)
		{
			int size = 2;
			glColor3f(0.0f, 1.0f, 0.0f);

			for (uint i = 0; i < vertex.size * 3; i += 3)
			{
				glBegin(GL_LINES);
				glVertex3f(vertex.data[i], vertex.data[i + 1], vertex.data[i + 2]);
				glVertex3f(vertex.data[i] + normals.data[i] * size, vertex.data[i + 1] + normals.data[i + 1] * size, vertex.data[i + 2] + normals.data[i + 2] * size);
				glEnd();
			}
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	}



	glBindBuffer(GL_ARRAY_BUFFER, uvs.id);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, index.size, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
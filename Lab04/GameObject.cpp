#include "GameObject.h"



void GameObject::draw()
{

	glBindVertexArray(model->VAO);
	update();


	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, globalLocation.m);

	glDrawArrays(GL_TRIANGLES, 0, model->modelData.mPointCount);
	Fog();

}

void GameObject::rotate_x(GLfloat amount)
{
	xyzDeg += vec3(amount, 0.0f, 0.0f);
	globalLocation = globalLocation*rotate_x_deg(identity_mat4(), amount);
}

void GameObject::rotate_y(GLfloat amount)
{
	xyzDeg += vec3(0.0f, amount, 0.0f);
	globalLocation = globalLocation*rotate_y_deg(identity_mat4(), amount);
}

void GameObject::rotate_z(GLfloat amount)
{
	xyzDeg += vec3(0.0f, 0.0f, amount);
	globalLocation = globalLocation*rotate_z_deg(identity_mat4(), amount);


}

void GameObject::translate_x(GLfloat amount)
{
	xyzPos += vec3(amount, 0.0f, 0.0f);
	globalLocation = globalLocation * translate(identity_mat4(), vec3(amount, 0.0f, 0.0f));
}

void GameObject::translate_y(GLfloat amount)
{
	xyzPos += vec3(0.0f, amount, 0.0f);
	globalLocation = globalLocation * translate(identity_mat4(), vec3(0.0f, amount, 0.0f));
}

void GameObject::translate_z(GLfloat amount)
{
	xyzPos += vec3(0.0f, 0.0f, amount);
	globalLocation = globalLocation * translate(identity_mat4(), vec3(0.0f, 0.0f, amount));


}

void GameObject::update()
{
	if (parent != NULL) {
		globalLocation = parent->globalLocation * translate(identity_mat4(),xyzPos) * rotate_x_deg(identity_mat4(), xyzDeg.v[0])
			* rotate_y_deg(identity_mat4(), xyzDeg.v[1]) * rotate_z_deg(identity_mat4(), xyzDeg.v[2]);;
	}
	else
	{
		globalLocation = identity_mat4()  * rotate_x_deg(identity_mat4(), xyzDeg.v[0])
			* rotate_y_deg(identity_mat4(), xyzDeg.v[1]) * rotate_z_deg(identity_mat4(), xyzDeg.v[2]) * translate(identity_mat4(), xyzPos);
	}

}

void GameObject::Fog()
{
	GLfloat fogcolor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.05f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 0.0f);
	glFogf(GL_FOG_END, 5.0f);
}


GameObject::GameObject()
{
	globalLocation = identity_mat4();
	xyzPos = vec3(0.0f, 0.0f, 0.0f);
	xyzDeg = vec3(0.0f, 0.0f, 0.0f);
	parent = NULL;
	matrix_location = glGetUniformLocation(model->shaderProgramID, "model_matrix");
	view_mat_location = glGetUniformLocation(model->shaderProgramID, "view_matrix");
	proj_mat_location = glGetUniformLocation(model->shaderProgramID, "proj_matrix");
	texture_num_loc = glGetUniformLocation(model->shaderProgramID, "texture_num");
	light_pos_loc = glGetUniformLocation(model->shaderProgramID, "light_pos_z");
	printf("Object Created\n");
}

GameObject::GameObject(Model *newModel, int textureIDnew)
{
	model = newModel;
	globalLocation = identity_mat4();
	textureID = textureIDnew;
	xyzPos = vec3(0.0f, 0.0f, 0.0f);
	xyzDeg = vec3(0.0f, 0.0f, 0.0f);
	parent = NULL;
	matrix_location = glGetUniformLocation(model->shaderProgramID, "model_matrix");
	view_mat_location = glGetUniformLocation(model->shaderProgramID, "view_matrix");
	proj_mat_location = glGetUniformLocation(model->shaderProgramID, "proj_matrix");
	texture_num_loc = glGetUniformLocation(model->shaderProgramID, "texture_num");
	light_pos_loc = glGetUniformLocation(model->shaderProgramID, "light_pos_z");
	printf("Object Created:%s\n", model->getName());
	update();
}

GameObject::GameObject(Model *newModel, int textureIDnew, vec3 Pos, vec3 Deg)
{
	model = newModel;
	globalLocation = identity_mat4();
	textureID = textureIDnew;
	xyzPos = Pos;
	xyzDeg = Deg;
	parent = NULL;
	matrix_location = glGetUniformLocation(model->shaderProgramID, "model_matrix");
	view_mat_location = glGetUniformLocation(model->shaderProgramID, "view_matrix");
	proj_mat_location = glGetUniformLocation(model->shaderProgramID, "proj_matrix");
	texture_num_loc = glGetUniformLocation(model->shaderProgramID, "texture_num");
	light_pos_loc = glGetUniformLocation(model->shaderProgramID, "light_pos_z");
	printf("Object Created:%s\n", model->getName());
	update();
}

GameObject::GameObject(Model * newModel, int textureIDnew, vec3 offset, vec3 degOffset, GameObject *parentNew)
{
	model = newModel;
	parent = parentNew;
	textureID = textureIDnew;

	xyzPos = offset;
	xyzDeg = degOffset;
	globalLocation = identity_mat4();

	matrix_location = glGetUniformLocation(model->shaderProgramID, "model_matrix");
	view_mat_location = glGetUniformLocation(model->shaderProgramID, "view_matrix");
	proj_mat_location = glGetUniformLocation(model->shaderProgramID, "proj_matrix");
	texture_num_loc = glGetUniformLocation(model->shaderProgramID, "texture_num");
	light_pos_loc = glGetUniformLocation(model->shaderProgramID, "light_pos_z");
	printf("Object Created:%s\n",model->getName());
	update();
}


GameObject::~GameObject()
{
	printf("Object Deleted\n");
}

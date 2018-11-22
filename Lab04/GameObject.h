#include "Model.h"
#pragma once
class GameObject
{
public:

	Model *model;
	GameObject *parent;

	mat4 globalLocation;

	//------------
	
	vec3 xyzPos;
	vec3 xyzDeg;

	//------------

	int matrix_location;
	int view_mat_location;
	int proj_mat_location;
	int texture_num_loc;
	int light_pos_loc;

	//texture id
	int textureID;
	
	//methods
	void draw();
	void rotate_x(GLfloat amount);
	void rotate_y(GLfloat amount);
	void rotate_z(GLfloat amount);
	void translate_x(GLfloat amount);
	void translate_y(GLfloat amount);
	void translate_z(GLfloat amount);
	void update();

	//constructors
	GameObject();
	GameObject(Model *newModel,int textureID);
	GameObject(Model *newModel,int textureID, vec3 offset,vec3 degOffset, GameObject *parentNew);
	~GameObject();
};


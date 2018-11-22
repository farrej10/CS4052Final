#include "maths_funcs.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma once
class Camera
{
public:
	mat4 view;
	mat4 resetview;
	mat4 persp_proj;

	mat4 globalLocation;

	//------------

	vec3 xyzPos;
	vec3 xyzDeg;

	//methods
	void rotate_x(GLfloat amount);
	void rotate_y(GLfloat amount);
	void rotate_z(GLfloat amount);
	void translate_x(GLfloat amount);
	void translate_y(GLfloat amount);
	void translate_z(GLfloat amount);
	void update();
	void reset();

	//constructors
	Camera();
	Camera(mat4 viewIn,mat4 perspectiveIn);
	~Camera();
};


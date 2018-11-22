#include "Camera.h"



void Camera::rotate_x(GLfloat amount)
{
	xyzDeg += vec3(amount, 0.0f, 0.0f);
}

void Camera::rotate_y(GLfloat amount)
{
	xyzDeg += vec3(0.0f, amount, 0.0f);
}

void Camera::rotate_z(GLfloat amount)
{
	xyzDeg += vec3(0.0f, 0.0f, amount);
}

void Camera::translate_x(GLfloat amount)
{
	xyzPos += vec3(amount, 0.0f, 0.0f);
}

void Camera::translate_y(GLfloat amount)
{
	xyzPos += vec3(0.0f, amount, 0.0f);
}

void Camera::translate_z(GLfloat amount)
{
	xyzPos += vec3(0.0f, 0.0f, amount);
}

void Camera::update()
{
	view = identity_mat4()  * rotate_x_deg(identity_mat4(), xyzDeg.v[0])
		* rotate_y_deg(identity_mat4(), xyzDeg.v[1]) * rotate_z_deg(identity_mat4(), xyzDeg.v[2]) * translate(identity_mat4(), xyzPos);
}

void Camera::reset()
{
	view = resetview;
	xyzPos = vec3(view.m[12], view.m[13], view.m[14]);
	xyzDeg = vec3(0.0f, 0.0f, 0.0f);
}

Camera::Camera()
{
}

Camera::Camera(mat4 viewIn, mat4 perspectiveIn)
{
	view = viewIn;
	
	resetview = view;
	xyzPos = vec3(view.m[12], view.m[13], view.m[14]);
	xyzDeg = vec3(0.0f, 0.0f, 0.0f);
	persp_proj = perspectiveIn;
}

Camera::~Camera()
{
}

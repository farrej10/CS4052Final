#include "Camera.h"



void Camera::rotate_x(GLfloat amount)
{
	view = rotate_x_deg(view, amount);
}

void Camera::rotate_y(GLfloat amount)
{
	view = rotate_y_deg(view, amount);
}

void Camera::rotate_z(GLfloat amount)
{
	view = rotate_z_deg(view, amount);
}

void Camera::translate_x(GLfloat amount)
{
	view = translate(view, vec3(0.0f, amount, 0.0f));
}

void Camera::translate_y(GLfloat amount)
{
	view = translate(view, vec3(amount, 0.0f, 0.0f));
}

void Camera::translate_z(GLfloat amount)
{
	view = translate(view, vec3(0.0f, 0.0f, amount));
}

void Camera::reset()
{
	view = resetview;
}

Camera::Camera()
{
}

Camera::Camera(mat4 viewIn, mat4 perspectiveIn)
{
	view = viewIn;
	resetview = view;
	persp_proj = perspectiveIn;
}

Camera::~Camera()
{
}

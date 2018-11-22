// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

// Project includes
#include "maths_funcs.h"
#pragma once
class ModelData
{
public:
	
	size_t mPointCount;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
	ModelData();
	~ModelData();
};

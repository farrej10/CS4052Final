#include <string>
#include "ModelData.h"
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "stb_image.h"
#pragma once

class Model
{
private:
	const char* MESH_NAME;
	const char* TEXTURE_NAME;
public:
	//attributes
	GLuint VAO;
	ModelData modelData;
	GLuint shaderProgramID;
	
	//methods
	bool load_mesh(const char* file_name);
	void generateObjectBufferMesh();
	const char* getName() { return MESH_NAME; }

	Model(const char* mesh_name,GLuint shaderProgramIDin);
	Model();
	~Model();
};


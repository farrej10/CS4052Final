// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.




// Assimp includes
#include "GameObject.h"
#include "Camera.h"




/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME_CACTUS "Meshes/cactuses2.obj"
#define MESH_NAME_LIZARD "Meshes/lizard6.obj"
#define MESH_NAME_LAND "Meshes/land2.obj"

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/


GLuint shaderProgramID;
GLuint textures[8];

Model *first;
Model *second;
Model *third;
Model *fourth;
Model *fifth;

Camera *camera;
GameObject *terrain;

int width = 800;
int height = 600;

std::vector<GameObject*> gameObjectList(6);

void Fog()
{

	GLfloat fogcolor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.05f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, -50.0f);
	glFogf(GL_FOG_END, 10.0f);
}

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "Shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "Shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

void loadTextures(GLuint texture, const char* filepath, int active_arg, const GLchar* texString, int texNum) {
	int x, y, n;
	int force_channels = 4;
	unsigned char *image_data = stbi_load(filepath, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", filepath);

	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr, "WARNING: texture %s is not power-of-2 dimensions\n",
			filepath);
	}

	glActiveTexture(active_arg);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		image_data);
	glUniform1i(glGetUniformLocation(shaderProgramID, texString), texNum);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

}



void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	Fog();
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//draw terrain
	glUniform1i(terrain->texture_num_loc, terrain->textureID);
	glBindTexture(GL_TEXTURE_2D, textures[terrain->textureID]);
	glUniformMatrix4fv(terrain->proj_mat_location, 1, GL_FALSE, camera->persp_proj.m);
	glUniformMatrix4fv(terrain->view_mat_location, 1, GL_FALSE, camera->view.m);
	terrain->draw();
	
	//draw objects
	for (int i = 0; i < gameObjectList.size();i++)
	{
		glUniform1i(gameObjectList[i]->texture_num_loc, gameObjectList[i]->textureID);
		glBindTexture(GL_TEXTURE_2D, textures[gameObjectList[i]->textureID]);
		glUniformMatrix4fv(gameObjectList[i]->proj_mat_location, 1, GL_FALSE, camera->persp_proj.m);
		glUniformMatrix4fv(gameObjectList[i]->view_mat_location, 1, GL_FALSE, camera->view.m);
		gameObjectList[i]->draw();
	}
	Fog();
	glutSwapBuffers();
	
}


void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time);
	

	//every 20ms(50fps) ish check for collision;
	if (delta >= 20)
	{

		if (camera->xyzPos.v[1] > -1.5f)
		{
			camera->xyzPos.v[1] = -1.5f;
		}
		camera->translate_y(0.05f);
		camera->update();
		//printf("%f y pos\n", camera->xyzPos.v[1]);
		last_time = curr_time;
	}

	

		

	
	glutPostRedisplay();
}


void init()
{
	srand(timeGetTime());
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	//gen and load textures textures
	glGenTextures(8, textures);

	loadTextures(textures[0], "Textures/red.jpg", GL_TEXTURE0, "redTexture", 0);
	loadTextures(textures[1], "Textures/black1.jpg", GL_TEXTURE1, "blackTexture", 1);
	loadTextures(textures[2], "Textures/grey.jpg", GL_TEXTURE2, "greyTexture", 2);
	loadTextures(textures[3], "Textures/cactus_texture3.jpg", GL_TEXTURE3, "cactusTexture", 3);
	loadTextures(textures[4], "Textures/sand_texture.jpg", GL_TEXTURE4, "sandTexture", 4);
	loadTextures(textures[5], "Textures/road_texture.jpg", GL_TEXTURE5, "roadTexture", 5);
	loadTextures(textures[6], "Textures/sky_texture2.jpg", GL_TEXTURE6, "skyTexture", 6);
	loadTextures(textures[7], "Textures/yellow.jpg", GL_TEXTURE7, "yellowTexture", 7);
	// load mesh into a vertex buffer array
	first = new Model(MESH_NAME_LAND, shaderProgramID);
	second = new Model(MESH_NAME_LIZARD,shaderProgramID);
	
	
	vec3 offset(0.0f, 2.5f, 0.0f);
	vec3 degOffset(0.0f, 0.0f, 0.0f);
	mat4 tempPer = identity_mat4();
	tempPer = translate(tempPer, vec3(0.0, -5.0, -25.0f));
	camera = new Camera(tempPer, perspective(70.0f, (float)width / (float)height, 0.1f, 1000.0f));
	terrain = new GameObject(first,4);
	for (int i = 0; i < gameObjectList.size(); i++)
	{
		int posOrNeg = rand() % 2 == 0 ? -1:1;
		int posOrNeg2 = rand() % 2 == 0 ? -1 : 1;
		gameObjectList[i] = new GameObject(second,3,vec3((rand() % 10)*posOrNeg, 1.0f, (rand() % 10)*posOrNeg2),vec3(0.0f,0.0f,0.0f));
	}

}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	
	
	
	if (key == '=') {
		//add new object to end of list with offset
		int posOrNeg = rand() % 2 == 0 ? -1 : 1;
		int posOrNeg2 = rand() % 2 == 0 ? -1 : 1;
		gameObjectList.push_back(new GameObject(second, 3, vec3((rand() % 10)*posOrNeg, 1.0f, (rand() % 10)*posOrNeg2), vec3(0.0f, 0.0f, 0.0f)));
	}
	if (key == '-') {
		//add new object to end of list with offset
		if (gameObjectList.size() != 0)
		{
			delete gameObjectList.back();
			gameObjectList.pop_back();
		}
	}

	if (key == '1') {
		//add new object to end of list with offset
		gameObjectList[0]->rotate_x(5.0f);
	}
	if (key == '2') {
		//add new object to end of list with offset
		gameObjectList[1]->rotate_z(5.0f);
	}


	if (key == 'w') {
		camera->translate_z(0.5f);
		camera->update();
	}
	if (key == 's') {
		//Translate the base, etc.
		camera->translate_z(-0.5f);
		camera->update();
	}
	if (key == 'd') {
		//Translate the base, etc.
		camera->translate_x(-0.5f);
		camera->update();
	}
	if (key == 'a') {
		//Translate the base, etc.
		camera->translate_x(0.5f);
		camera->update();
	}
	if (key == 'e') {
		//Translate the base, etc.
		camera->translate_y(0.5f);
		camera->update();
	}
	if (key == 'q') {
		//Translate the base, etc.
		camera->translate_y(-0.5f); 
		camera->update();
	}
	

	if (key == 'u') {
		//Translate the base, etc.
		camera->rotate_x(5.0f);
		camera->update();
	}
	if (key == 'j') {
		//Translate the base, etc.
		camera->rotate_x(-5.0f);
		camera->update();
	}
	if (key == 'k') {
		//Translate the base, etc.
		camera->rotate_y(5.0f);
		camera->update();
	}
	if (key == 'h') {
		//Translate the base, etc.
		camera->rotate_y(-5.0f);
		camera->update();
	}
	if (key == 'i') {
		//Translate the base, etc.
		camera->rotate_z(5.0f);
		camera->update();
	}
	if (key == 'y') {
		//Translate the base, etc.
		camera->rotate_z(-5.0f);
		camera->update();
	}
	if (key == '0') {
		//Translate the base, etc.
		camera->reset();
	}
	if (key == 27) {
		//Translate the base, etc.
		glutExit();
	}
	
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("James Farrell");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}

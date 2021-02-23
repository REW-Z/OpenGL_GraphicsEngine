#pragma once

using namespace std;

float toRadians(float degrees);
void PrintMatrix(glm::mat4 mat);
bool checkOpenGLError();
void printProgramLog(int program);
void printShaderLog(GLuint shader);
string readShaderSource(const char * path);
GLuint createShaderProgram(const char * vertPath, const char * fragPath);
GLuint loadTexture(const char * texImagePath);
GLuint loadCubeMap(const char *mapDir);


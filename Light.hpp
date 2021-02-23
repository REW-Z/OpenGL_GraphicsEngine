#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2/SOIL2.h>
#include <cmath>
#include <stack>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace glm;

class Light
{

};

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	vec3 dir;
	vec4 color;
};

DirectionalLight::DirectionalLight()
{
	dir = vec3(0.0f, -1.0f, 0.0f);
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
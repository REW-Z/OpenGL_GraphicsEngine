#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2/SOIL2.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

#include <cmath>
#include <stack>
#include <vector>
#include <list>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Geometry.hpp"

#include "Object.h"

#include "BaseComponents.h"

#include "GameObject.h"

using namespace std;

int GameObject::tid() const
{
	return 1;
}

GameObject::GameObject()
{
	Transform * t = new Transform();
	components.push_back(t);
	transform = t;
}

GameObject::~GameObject()
{
	delete transform;
}

Component * GameObject::getComponent(ObjectType type)
{
	Component * componentPtr = nullptr;
	for (vector<Component *>::iterator it = components.begin(); it != components.end(); (it)++)
	{
		if ((*it)->tid() == (int)type)
		{
			componentPtr = *it;
		}
	}
	return componentPtr;
}

void GameObject::addComponent(Component * c)
{
	components.push_back(c);
}
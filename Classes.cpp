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
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Geometry.hpp"

#include "Object.h"

#include "Classes.h"


using namespace std;
using namespace glm;



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


int Component::tid() const
{
	return 2;
}

Component::Component() { this->gameObject = nullptr; }

int Transform::tid() const
{
	return 3;
}

Transform::Transform()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	parent = nullptr;
}

mat4 Transform::getMatrixM()
{
	mat4 mS = glm::scale(mat4(1.0f), vec3(scale.x, scale.y, scale.z));
	mat4 mRz = glm::rotate(mat4(1.0f), (rotation.z * pi<float>()) / 180.0f, vec3(0.0f, 0.0f, 1.0f));
	mat4 mRx = glm::rotate(mat4(1.0f), (rotation.x * pi<float>()) / 180.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 mRy = glm::rotate(mat4(1.0f), (rotation.y * pi<float>()) / 180.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 mT = glm::translate(mat4(1.0f), vec3(position.x, position.y, position.z));

	mat4 result = (mT * mRy * mRx * mRz * mS);
	if (parent != nullptr)
	{
		return (parent->getMatrixM()) * result;
	}
	else
	{
		return result;
	}
}






int MonoScript::tid() const { return 4; }

MonoScript::MonoScript(const char * name)
{
	//this->objInstance = nullptr;

	//MonoClass * monoclassCurrentScript = mono_class_from_name(image_scripts, "MonoCSharp", name);
	//if (monoclassCurrentScript == nullptr)
	//{
	//	//cout << "didn't find class in Assembly_Scripts, now query in Assembly_Engine..." << endl;
	//	monoclassCurrentScript = mono_class_from_name(image_core, "MonoCSharp", name);
	//}
	//if (monoclassCurrentScript == nullptr)
	//{
	//	//cout << "no class founded!!!" << endl;
	//	return;
	//}
	//this->objInstance = mono_object_new(domain, monoclassCurrentScript);

	//MonoClassField * field = NULL;
	//void * itor = NULL;

	//while (field = mono_class_get_fields(monoclassCurrentScript, &itor))
	//{
	//	const char * field_name = mono_field_get_name(field);
	//	MonoType * type = mono_field_get_type(field);
	//	MonoTypeEnum typeEnum = static_cast<MonoTypeEnum> (mono_type_get_type(type));

	//	switch (typeEnum)
	//	{
	//	case MONO_TYPE_OBJECT:
	//	{
	//		MonoClass * monoclassTmp = mono_class_from_mono_type(type);
	//		MonoObject * obj = mono_object_new(domain, monoclassTmp);
	//		mono_field_set_value(objInstance, field, &obj);
	//		break;
	//	}
	//	default:
	//		break;
	//	}

	//}
}


int Scene::tid() const { return 999; }
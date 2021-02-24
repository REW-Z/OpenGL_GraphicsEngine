#pragma once 


class Component : public Object
{
public:
	virtual int tid() const;//2

	GameObject * gameObject;

	Component();
};

class Transform : public Component
{
public:
	int tid() const;//3
	Transform();

	Transform * parent;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	glm::mat4 getMatrixM();
};

class MonoScript : public Component
{
public:
	int tid() const;
	MonoObject * objInstance;
	MonoScript(const char * name, MonoDomain* domain, MonoImage* image_core, MonoImage* image_scripts);
};

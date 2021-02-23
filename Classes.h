#pragma once 



class GameObject : public Object
{
public:
	int tid() const;//1

	GameObject();
	~GameObject();

	const char * name;
	Transform * transform;

	std::vector<Component *> components;

	Component * getComponent(ObjectType type);
	void addComponent(Component * c);
};

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
	MonoScript(const char * name);
};


class Scene
{
public:
	int tid() const;
	std::vector<GameObject *> gameObjects;
};
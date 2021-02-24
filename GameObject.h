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


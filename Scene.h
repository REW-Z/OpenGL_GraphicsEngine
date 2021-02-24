#pragma once


class Scene
{
public:
	int tid() const;
	std::list<GameObject *> gameObjects;
};


#pragma once

//---
class Object;
class GameObject;
class Component;
class Transform;
class MonoScript;
//---

enum ObjectType
{
	enum_Object = 0,
	enum_GameObject = 1,
	enum_Component = 2,
	enum_Transform = 3,
	enum_MonoScript = 4,
	enum_Mesh = 5,
	enum_Scene = 999
};



//enum ComponentType
//{
//	enum_component = 0,
//	enum_transform = 1,
//	enum_mesh = 2
//};

class Object
{
public:
	virtual int tid() const;//0
};


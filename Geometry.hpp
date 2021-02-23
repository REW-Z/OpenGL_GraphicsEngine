#pragma once

//Vec3结构体定义
struct Vector3
{
public:

	float x;
	float y;
	float z;
	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float newx, float newy, float newz) { x = newx; y = newy; z = newz; }
};
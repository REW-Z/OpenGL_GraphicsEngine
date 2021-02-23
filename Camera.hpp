#pragma once
using namespace std;
using namespace glm;

//***************************************************************************************
//GLM和GLSL使用mat4都是按列排列存储的，需要transpose转置一下。
//
//***************************************************************************************

class Camera
{
public:
	Camera();
	glm::vec3 e;//position  （translate To 0,0,0）
	glm::vec3 g;//forward  （rotate To -z）
	glm::vec3 t;//up  （rotate To +y）
	glm::vec3 gxt();// { get{ return Vector3.Cross(g, t); } } //right  （rotate To +x）

	float near; // near = 1 / tan(fov / 2)
	float far;
	float aspectRatio;
	float fov;

	mat4 GetMatrixV();
	mat4 glmGetMatrixV();
	mat4 GetMatrixP();
	mat4 GetMatrixVP();
};

Camera::Camera()
{
	e = vec3(0.0, 0.0, 0.0);
	g = vec3(0.0, 0.0, -1.0);
	t = vec3(0.0, 1.0, 0.0);

	far = 1000.0f;
	fov = 90.0f;
	near = 0.1f;
	aspectRatio = 1.0f;
}

vec3 Camera::gxt()
{
	return cross(g, t);
}



//V视图变换矩阵
mat4 Camera::GetMatrixV()
{
	mat4 mTview = transpose(mat4(
		1.0f, 0.0f, 0.0f, -e.x,
		0.0f, 1.0f, 0.0f, -e.y,
		0.0f, 0.0f, 1.0f, -e.z,
		0.0f, 0.0f, 0.0f, 1.0f
	));
	mat4 mRview = transpose(mat4(
		gxt().x, gxt().y, gxt().z, 0.0f,
		t.x, t.y, t.z, 0.0f,
		-g.x, -g.y, -g.z, 0.0f, //g方向朝向-z方向
		0.0f, 0.0f, 0.0f, 1.0f
	));
	mat4 mV = mRview * mTview;

	return mV;
}

mat4 Camera::glmGetMatrixV()
{
	mat4 m = lookAt(e, e + g, vec3(0.0f, 1.0f, 0.0f));

	return m;
}


//P投影变换矩阵
mat4 Camera::GetMatrixP()
{
	float fovR = (glm::pi<float>() / 180.0f) * fov;

	mat4 mP = transpose(mat4(
		(1.0f / tan(fovR / 2.0f)) / aspectRatio, 0.0f, 0.0f, 0.0f,
		0.0f, (1.0f / tan(fovR / 2.0f)), 0.0f, 0.0f,
		0.0f, 0.0f, (near + far) / (near - far), (2.0f * near * far) / (near - far),
		0.0f, 0.0f, -1.0f, 0.0f
	));

	return mP;
}

mat4 Camera::GetMatrixVP()
{
	return GetMatrixP()*GetMatrixV();
}
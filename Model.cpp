#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <stack>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include "Utils.h"

#include "Geometry.hpp"

#include "Object.h"

#include "Classes.h"

#include "Model.h"

using namespace std;
using namespace glm;

//---------------------------------------------------------------------

#pragma region IndexLessMeshImporter
IndexlessModelImporter::IndexlessModelImporter() {}

void IndexlessModelImporter::parseOBJ(const char *filePath)
{
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0)
		{
			stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z;
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}
		if (line.compare(0, 2, "vt") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y;
			stVals.push_back(x);
			stVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}
		if (line.compare(0, 2, "f ") == 0)
		{
			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++)
			{
				getline(ss, oneCorner, ' ');
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

				int vertRef = 0;
				int tcRef = 0;
				int normRef = 0;

				if (v != "") vertRef = (stoi(v) - 1) * 3;
				if (t != "") tcRef = (stoi(t) - 1) * 2;
				if (n != "") normRef = (stoi(n) - 1) * 3;

				triangleVerts.push_back(vertVals[vertRef]);
				triangleVerts.push_back(vertVals[vertRef + 1]);
				triangleVerts.push_back(vertVals[vertRef + 2]);

				textureCoords.push_back(stVals[tcRef]);
				textureCoords.push_back(stVals[tcRef + 1]);

				normals.push_back(normVals[normRef]);
				normals.push_back(normVals[normRef + 1]);
				normals.push_back(normVals[normRef + 2]);
			}
		}
	}
	fileStream.close();
}
int IndexlessModelImporter::getNumVertices() { return (triangleVerts.size() / 3); }
std::vector<float> IndexlessModelImporter::getVertices() { return triangleVerts; }
std::vector<float> IndexlessModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> IndexlessModelImporter::getNormals() { return normals; }
#pragma endregion


//---------------------------------------------------------------------

#pragma region MeshImporter
ModelImporter::ModelImporter() {}

void ModelImporter::parseOBJ(const char *filePath)
{
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	
	// define vars about mesh part
	int indexPart = -1;//index of current part
	vector<int> * meshPartIndsPtr = nullptr; //part inds array pointer 

	// define vars about vert/tc/norm/tangent
	vector<ivec4> v_t_n;//vtc数组长度为处理后可索引顶点数。
	bool isExist;

	vec3 pos_temp[3];
	vec2 tc_temp[3];
	vec3 norm_temp[3];
	bool need_push[3];
	vec3 tangent1, tangent2, tangent3;

	while (!fileStream.eof())
	{
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0)
		{
			stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z;
			rawVertVals.push_back(x);
			rawVertVals.push_back(y);
			rawVertVals.push_back(z);
		}
		if (line.compare(0, 2, "vt") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y;
			rawTexcoordsVals.push_back(x);
			rawTexcoordsVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			rawNormVals.push_back(x);
			rawNormVals.push_back(y);
			rawNormVals.push_back(z);
		}
		if (line.compare(0, 7, "usemtl ") == 0)
		{
			MeshPart part;
			Material mat = Material();
			mat.hasDiffTex = false;
			mat.hasBumpTex = false;
			mat.name = line.erase(0, 7);
			part.material = mat;
			meshParts.push_back(part); indexPart++;
			meshPartIndsPtr = &(meshParts[indexPart].inds);
		}
		if (line.compare(0, 2, "f ") == 0)
		{
			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++)
			{
				getline(ss, oneCorner, ' ');
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

				int vertInd = 0;
				int tcInd = 0;
				int normInd = 0;

				if (v != "") vertInd = stoi(v);
				if (t != "") tcInd = stoi(t);
				if (n != "") normInd = stoi(n);

				//顶点数据查找检索（从原始数据）
				pos_temp[i].x = rawVertVals[(vertInd - 1) * 3];
				pos_temp[i].y = rawVertVals[(vertInd - 1) * 3 + 1];
				pos_temp[i].z = rawVertVals[(vertInd - 1) * 3 + 2];

				tc_temp[i].x = rawTexcoordsVals[(tcInd - 1) * 2];
				tc_temp[i].y = rawTexcoordsVals[(tcInd - 1) * 2 + 1];

				norm_temp[i].x = rawNormVals[(normInd - 1) * 3];
				norm_temp[i].y = rawNormVals[(normInd - 1) * 3 + 1];
				norm_temp[i].z = rawNormVals[(normInd - 1) * 3 + 2];

				//判断
				isExist = false;
				int ind = 0;
				for (size_t i = 0; i < v_t_n.size(); i++)
				{
					if (v_t_n[i].x != vertInd)
					{
						break;
					}
					if (v_t_n[i].y != tcInd)
					{
						break;
					}
					if (v_t_n[i].z == normInd)
					{
						isExist = true;
						ind = v_t_n[i].w;
					}
				}
				//根据判断结果
				if (!isExist)
				{
					need_push[i] = true;

					triangleVerts.push_back(pos_temp[i].x);
					triangleVerts.push_back(pos_temp[i].y);
					triangleVerts.push_back(pos_temp[i].z);

					textureCoords.push_back(tc_temp[i].x);
					textureCoords.push_back(tc_temp[i].y);

					normals.push_back(norm_temp[i].x);
					normals.push_back(norm_temp[i].y);
					normals.push_back(norm_temp[i].z);
					//v_t_n存储
					int lastIndex = v_t_n.size() - 1;
					v_t_n.push_back(ivec4(vertInd, tcInd, normInd, lastIndex + 1));
					//总索引和分部索引存储
					inds.push_back(lastIndex + 1);
					(*meshPartIndsPtr).push_back(lastIndex + 1);
				}
				else
				{
					need_push[i] = false;

					//总索引和分部索引压入
					inds.push_back(ind);
					(*meshPartIndsPtr).push_back(ind);
				}
				//pos/tc/norm read end
			}
			//triangle read end
			//面信息读取后 分别计算三个切线
			tangent1 = GetTangent(pos_temp[0], pos_temp[1], pos_temp[2], tc_temp[0], tc_temp[1], tc_temp[2], norm_temp[0]);
			tangent2 = GetTangent(pos_temp[1], pos_temp[2], pos_temp[0], tc_temp[1], tc_temp[2], tc_temp[0], norm_temp[1]);
			tangent3 = GetTangent(pos_temp[2], pos_temp[0], pos_temp[1], tc_temp[2], tc_temp[0], tc_temp[1], norm_temp[2]);
			if (need_push[0])
			{
				tangents.push_back(tangent1.x);
				tangents.push_back(tangent1.y);
				tangents.push_back(tangent1.z);
			}
			if (need_push[1])
			{
				tangents.push_back(tangent2.x);
				tangents.push_back(tangent2.y);
				tangents.push_back(tangent2.z);
			}
			if (need_push[2])
			{
				tangents.push_back(tangent3.x);
				tangents.push_back(tangent3.y);
				tangents.push_back(tangent3.z);
			}
		}
		//line read end
	}
	fileStream.close();
}
void ModelImporter::parseMTL(const char *filePath)
{
	string objPath = filePath;
	string mtlPath = objPath.substr(0, objPath.rfind(".")) + ".mtl";
	string dir = objPath.substr(0, objPath.rfind("\\"));

	ifstream fileStream(mtlPath, ios::in);
	string line = "";
	MeshPart * currentPart = nullptr;
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		if (line.compare(0, 7, "newmtl ") == 0)
		{
			string name = line.erase(0, 7);
			for (size_t i = 0; i < meshParts.size(); i++)
			{
				if (meshParts[i].material.name == name)
				{
					currentPart = &meshParts[i];
				}
			}
		}
		if (line.compare(0, 2, "Ka") == 0)
		{
			float x, y, z;
			stringstream ss(line.erase(0, 3));
			ss >> x;
			ss >> y;
			ss >> z;
			(*currentPart).material.ambient = vec3(x, y, z);
		}
		if (line.compare(0, 2, "Kd") == 0)
		{
			float x, y, z;
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			(*currentPart).material.diffuse = vec3(x, y, z);
		}
		if (line.compare(0, 2, "Ks") == 0)
		{
			float x, y, z;
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			(*currentPart).material.specular = vec3(x, y, z);
		}
		if (line.compare(0, 2, "Ke") == 0)
		{
			float x, y, z;
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			(*currentPart).material.emission = vec3(x, y, z);
		}
		if (line.compare(0, 2, "Ns") == 0)
		{
			float Ns = stof(line.erase(0, 2));
			(*currentPart).material.Ns = Ns;
		}
		if (line.compare(0, 2, "Ni") == 0)
		{

		}
		if (line.compare(0, 2, "d ") == 0)
		{

		}
		if (line.compare(0, 6, "map_Kd") == 0)
		{
			string texpath = dir + "\\" + line.erase(0, 7);
			currentPart->material.textureDiffuse = loadTexture(texpath.c_str());
			currentPart->material.hasDiffTex = true;
		}
		if (line.compare(0, 8, "map_Bump") == 0)
		{
			string texpath = dir + "\\" + line.erase(0, 9);
			currentPart->material.textureBump = loadTexture(texpath.c_str());
			currentPart->material.hasBumpTex = true;
		}
	}
	fileStream.close();
}
int ModelImporter::getNumVertices() { return (triangleVerts.size() / 3); }
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }
std::vector<float> ModelImporter::getTangents() { return tangents; }
std::vector<int> ModelImporter::getInds() { return inds; }
std::vector<MeshPart> ModelImporter::getMeshParts() { return meshParts; }
#pragma endregion



//---------------------------------------------------------------------

#pragma region IndexlessMesh
int IndexlessMesh::tid()
{
	return 5;
}

IndexlessMesh::IndexlessMesh()
{
	bufferId = -1;
}

IndexlessMesh::IndexlessMesh(const char *filePath)
{
	IndexlessModelImporter modelImporter = IndexlessModelImporter();
	modelImporter.parseOBJ(filePath);
	numVertices = modelImporter.getNumVertices();
	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoordinates();
	std::vector<float> normals = modelImporter.getNormals();

	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}

	bufferId = -1;
}

int IndexlessMesh::getNumVertices() { return numVertices; }
std::vector<glm::vec3> IndexlessMesh::getVertices() { return vertices; }
std::vector<glm::vec2> IndexlessMesh::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> IndexlessMesh::getNormals() { return normalVecs; }
#pragma endregion


//------------------------------------------------------------------------------

#pragma region Mesh
int Mesh::tid() const
{
	return 5;
}

Mesh::Mesh()
{
	bufferId = -1;
}

Mesh::Mesh(const char * filePath)
{
	ModelImporter modelImporter = ModelImporter();

	modelImporter.parseOBJ(filePath);
	modelImporter.parseMTL(filePath);

	numVertices = modelImporter.getNumVertices();
	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoordinates();
	std::vector<float> normals = modelImporter.getNormals();
	std::vector<float> tangents = modelImporter.getTangents();

	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
		tangentVecs.push_back(glm::vec3(tangents[i * 3], tangents[i * 3 + 1], tangents[i * 3 + 2]));
	}
	inds = modelImporter.getInds();
	meshParts = modelImporter.getMeshParts();

	bufferId = -1;
}

int Mesh::getNumVertices() { return numVertices; }
std::vector<glm::vec3> Mesh::getVertices() { return vertices; }
std::vector<glm::vec2> Mesh::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> Mesh::getNormals() { return normalVecs; }
std::vector<glm::vec3> Mesh::getTangents() { return tangentVecs; }
std::vector<int> Mesh::getIndicates() { return inds; }
int Mesh::getNumIndicates() { return inds.size(); }
std::vector<MeshPart> Mesh::getMeshParts() { return meshParts; }
#pragma endregion


//--------------------------- Functions ----------------------------------------

glm::vec3 GetTangent(vec3 pos1, vec3 pos2, vec3 pos3, vec2 uv1, vec2 uv2, vec2 uv3, vec3 normal)
{
	glm::vec3 tangent;

	vec3 edge1 = pos2 - pos1;
	vec3 edge2 = pos3 - pos1;
	vec2 deltaUV1 = uv2 - uv1;
	vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = normalize(tangent);

	//glm::vec3 bitangent;

	//bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	//bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	//bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	//bitangent = normalize(bitangent);

	return tangent;
}
#pragma once
#include <vector>


//----------------------------------------------------------
//1、推荐使用自定义的模型文件类型，提高读取速度，而不是每次都导入obj文件。
//-----------------------class define-----------------------

class Material
{
public:
	std::string name;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emission;
	float Ns;

	bool hasDiffTex;
	bool hasBumpTex;
	GLuint textureDiffuse;
	GLuint textureBump;
};

class MeshPart
{
public:
	Material material;
	std::vector<int> inds;
};


// -------------------------class define-----------------------

class IndexlessModelImporter
{
private:
	std::vector<float> vertVals;
	std::vector<float> stVals;
	std::vector<float> normVals;

	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;
public:
	IndexlessModelImporter();
	void parseOBJ(const char *filePath);
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoordinates();
	std::vector<float> getNormals();
};

class ModelImporter
{
private:
	std::vector<float> rawVertVals;
	std::vector<float> rawTexcoordsVals;
	std::vector<float> rawNormVals;

	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;
	std::vector<float> tangents;
	
	std::vector<int> inds;
	std::vector<MeshPart> meshParts;
public:
	ModelImporter();
	void parseOBJ(const char *filePath);
	void parseMTL(const char *filePath);
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoordinates();
	std::vector<float> getNormals();
	std::vector<float> getTangents();
	std::vector<int> getInds();
	std::vector<MeshPart> getMeshParts();
};




//-----------------------class define-----------------------
class IndexlessMesh: public Component
{
protected:
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;
public:
	int tid();
	IndexlessMesh();
	IndexlessMesh(const char *filePath);

	int bufferId;

	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormals();
};

class Mesh: public Component
{
protected:
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;
	std::vector<glm::vec3> tangentVecs;
	std::vector<int> inds;
	std::vector<MeshPart> meshParts;
public:
	int tid() const;
	Mesh();
	Mesh(const char *filePath);

	int bufferId;

	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();
	std::vector<int> getIndicates();
	int getNumIndicates();
	std::vector<MeshPart> getMeshParts();
};

//--------------------- Functions ----------------------------------
glm::vec3 GetTangent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec3 normal);






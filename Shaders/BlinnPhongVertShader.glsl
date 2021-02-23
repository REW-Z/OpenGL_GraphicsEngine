#version 430

struct DirectionalLight
{
	vec4 color;
	vec3 dir;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float Ns;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;



uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;

uniform vec4 env_ambient;
uniform DirectionalLight light;
uniform Material materials;


layout(binding = 0) uniform sampler2DShadow shadowTex;
layout(binding = 1) uniform samplerCube envTex;
layout(binding = 2) uniform sampler2D samp1;
layout(binding = 3) uniform sampler2D samp2;

out vec3 varyingPos;
out vec3 varyingNormal;
out vec3 varyingTangent;
out vec2 uv;
out vec4 shadow_coord;

void main(void)
{ 
	varyingPos = (mv_matrix * vec4(position, 1.0)).xyz;
	varyingNormal = (norm_matrix * vec4(normal, 1.0)).xyz;
	varyingTangent = (norm_matrix * vec4(tangent, 1.0)).xyz;
	shadow_coord = shadowMVP * vec4(position,1.0);
	uv = texcoords;
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}
#version 430

layout (location = 0) in vec3 position;
out vec3 cubetc;

uniform mat4 v_matrix;
uniform mat4 p_matrix;
layout (binding = 0) uniform samplerCube samp;

void main(void)
{
	cubetc = position;//纹理坐标就是顶点坐标
	mat4 v3_matrix = mat4(mat3(v_matrix));//去除V矩阵中的平移
	gl_Position = p_matrix * v3_matrix * vec4(position,1.0);
}

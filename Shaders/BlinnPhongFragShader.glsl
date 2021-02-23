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


in vec3 varyingPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec2 uv;
in vec4 shadow_coord;

out vec4 color;

uniform int window_width;
uniform int window_height;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;

uniform vec4 env_ambient;
uniform DirectionalLight light;
uniform Material material;

layout(binding = 0) uniform sampler2DShadow shadowTex;
layout(binding = 1) uniform samplerCube envTex;
layout(binding = 2) uniform sampler2D samp1;
layout(binding = 3) uniform sampler2D samp2;

float sampShadowTex(float ox, float oy)
{
	//第三个参数也可以避免阴影痤疮。（和glPolygonOffset()作用一样）
	 return textureProj(shadowTex, shadow_coord + vec4(ox * (1.0 / window_width) * shadow_coord.w, oy * (1.0 / window_height) * shadow_coord.w, -0.005, 0.0));
}


vec3 calcNewNormal()
{
	vec3 normal = normalize(varyingNormal);
	vec3 tangent = normalize(varyingTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 retrievedNormal = texture(samp2, uv).xyz;
	retrievedNormal = retrievedNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);
	return newNormal;
}


void main(void)
{
	vec3 Nview = calcNewNormal();
	vec3 Lview = normalize((v_matrix * vec4(-light.dir, 0.0)).xyz);
	vec3 Vview = normalize(-varyingPos);
	vec3 Hview = normalize(Lview + Vview);
	vec3 Rview = reflect(normalize(varyingPos), normalize(varyingNormal));

	vec3 Rworld = (vec4(Rview, 0.0) * v_matrix).xyz;

	vec4 env_color = texture(envTex,Rworld);


	//----------------光照计算--------------------------
	float gloss = material.Ns * 2;
	float smoothnessFactor = clamp(material.Ns / 1000, 0.0, 1.0);

	vec3 ambient = ((env_ambient * material.ambient) * (env_color * smoothnessFactor + vec4(0.1, 0.1, 0.1, 1.0) * (1.0 - smoothnessFactor))).xyz;
	vec3 diffuse = light.color.xyz * material.diffuse.xyz * texture(samp1, uv).xyz * max(0.0, dot(Nview, Lview));
	vec3 specular = light.color.xyz * material.specular.xyz * pow(max(0.0, dot(Hview, Nview)), gloss); 
	
	//----------------阴影---------------------------------
	float shadowFactor = 0.0;
	float swidth = 1.0;
	vec2 offset = mod(floor(gl_FragCoord.xy), 2.0);//抖动值。有四个值(0,0)(0,1)(1,0)(1,1)
	shadowFactor += sampShadowTex((-1.5 + offset.x) * swidth, (1.5 - offset.y) * swidth);
	shadowFactor += sampShadowTex((-1.5 + offset.x) * swidth, (-0.5 - offset.y) * swidth);
	shadowFactor += sampShadowTex((0.5 + offset.x) * swidth, (1.5 - offset.y) * swidth);
	shadowFactor += sampShadowTex((0.5 + offset.x) * swidth, (-0.5 - offset.y) * swidth);
	shadowFactor = shadowFactor / 4.0;

	//----------------颜色输出-----------------------------
	color = vec4(ambient + shadowFactor * (diffuse + specular), 1.0);

//  //硬阴影
//	float inShadow = textureProj(shadowTex, shadow_coord);
//	if (inShadow != 0.0)
//	{
//		color = vec4((ambient.xyz + diffuse + specular), 1.0); 
//	}
//	else
//	{
//		color = vec4((ambient.xyz), 1.0); 
//	}
	//vec3 worldTangent = normalize((vec4(varyingTangent, 0.0) * v_matrix).xyz);

	//color = vec4((vec3(0.5, 0.5, 0.5) + (worldTangent * 0.5)), 1.0);
}
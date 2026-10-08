
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

#if HAS_AMBIENT_MAP == 1
out vec2 ambientTexCoord;
#endif

#if HAS_DIFFUSE_MAP == 1
out vec2 diffuseTexCoord;
#endif

#if HAS_SPECULAR_MAP == 1
out vec2 specularTexCoord;
#endif

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	Normal = normalMatrix * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
#if HAS_AMBIENT_MAP == 1
	ambientTexCoord = aTexCoord;
#endif
#if HAS_DIFFUSE_MAP == 1
	diffuseTexCoord = aTexCoord;
#endif
#if HAS_SPECULAR_MAP == 1
	specularTexCoord = aTexCoord;
#endif
}

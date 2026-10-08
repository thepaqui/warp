
struct Material {
// Ambient component
#if HAS_AMBIENT_MAP == 1
	sampler2D	ambientMap;
#else
	vec3		ambientColor;
#endif
};

// Point Light structure
#if POINT_LIGHT_COUNT > 0
struct PointLight {
	vec3	position;
	vec3	ambientIntensity;
	float	attenuationConstant;
	float	attenuationLinear;
	float	attenuationQuadratic;
};

uniform PointLight[POINT_LIGHT_COUNT] pointLight;

vec3	calcPointLight(PointLight light, vec3 fragPos, vec3 ambientColor);
#endif

// Directional Light structure
#if DIR_LIGHT_COUNT > 0
struct DirLight {
	vec3 direction;
	vec3 ambientIntensity;
};

uniform DirLight[DIR_LIGHT_COUNT] directionalLight;

vec3	calcDirLight(DirLight light, vec3 ambientColor);
#endif

// Spot Light structure
#if SPOT_LIGHT_COUNT > 0
struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	vec3 ambientIntensity;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform SpotLight[SPOT_LIGHT_COUNT] spotLight;

vec3	calcSpotLight(SpotLight light, vec3 fragPos, vec3 ambientColor);
#endif

uniform Material material;

in vec3 FragPos;
out vec4 FragColor;

#if HAS_AMBIENT_MAP == 1
in vec2 ambientTexCoord;
#endif

void	main()
{
	vec3	ambientColor;
#if HAS_AMBIENT_MAP == 1
	ambientColor = texture(material.ambientMap, ambientTexCoord).rgb;
#else
	ambientColor = material.ambientColor;
#endif

	// Directional Light
	vec3 dirColor = vec3(0.0);
#if DIR_LIGHT_COUNT > 0
	for (int i = 0; i < DIR_LIGHT_COUNT; i++)
		dirColor += calcDirLight(directionalLight[i], ambientColor);
#endif

	// Point Light
	vec3 pointColor = vec3(0.0);
#if POINT_LIGHT_COUNT > 0
	for (int i = 0; i < POINT_LIGHT_COUNT; i++)
		pointColor += calcPointLight(pointLight[i], FragPos, ambientColor);
#endif

	// Spot Light
	vec3 spotColor = vec3(0.0);
#if SPOT_LIGHT_COUNT > 0
	for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
		spotColor += calcSpotLight(spotLight[i], FragPos, ambientColor);
#endif

	// Final color
	vec3 finalColor = dirColor + pointColor + spotColor;

	FragColor = vec4(finalColor, 1.0);
}

#if DIR_LIGHT_COUNT > 0
vec3	calcDirLight(DirLight light, vec3 ambientColor)
{
	vec3	ambient = light.ambientIntensity * ambientColor;

	return (ambient);
}
#endif

#if POINT_LIGHT_COUNT > 0
vec3	calcPointLight(PointLight light, vec3 fragPos, vec3 ambientColor)
{
	vec3	ambient = light.ambientIntensity * ambientColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;

	return (ambient);
}
#endif

#if SPOT_LIGHT_COUNT > 0
vec3	calcSpotLight(SpotLight light, vec3 fragPos, vec3 ambientColor)
{
	vec3	ambient = light.ambientIntensity * ambientColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;

	return (ambient);
}
#endif

struct Material {
// Ambient component
#if HAS_AMBIENT_MAP == 1
	sampler2D	ambientMap;
#else
	vec3		ambientColor;
#endif

// Diffuse component
#if HAS_DIFFUSE_MAP == 1
	sampler2D	diffuseMap;
#else
	vec3		diffuseColor;
#endif
};

// Point Light structure
#if POINT_LIGHT_COUNT > 0
struct PointLight {
	vec3	position;
	vec3	ambientIntensity;
	vec3	diffuseIntensity;
	float	attenuationConstant;
	float	attenuationLinear;
	float	attenuationQuadratic;
};

uniform PointLight[POINT_LIGHT_COUNT] pointLight;

vec3	calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 ambientColor, vec3 diffuseColor);
#endif

// Directional Light structure
#if DIR_LIGHT_COUNT > 0
struct DirLight {
	vec3 direction;
	vec3 ambientIntensity;
	vec3 diffuseIntensity;
};

uniform DirLight[DIR_LIGHT_COUNT] directionalLight;

vec3	calcDirLight(DirLight light, vec3 normal, vec3 ambientColor, vec3 diffuseColor);
#endif

// Spot Light structure
#if SPOT_LIGHT_COUNT > 0
struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	vec3 ambientIntensity;
	vec3 diffuseIntensity;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform SpotLight[SPOT_LIGHT_COUNT] spotLight;

vec3	calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 ambientColor, vec3 diffuseColor);
#endif

uniform Material material;

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

#if HAS_AMBIENT_MAP == 1
in vec2 ambientTexCoord;
#endif

#if HAS_DIFFUSE_MAP == 1
in vec2 diffuseTexCoord;
#endif

void	main()
{
	vec3 norm = normalize(Normal);

	vec3	ambientColor;
#if HAS_AMBIENT_MAP == 1
	ambientColor = texture(material.ambientMap, ambientTexCoord).rgb;
#else
	ambientColor = material.ambientColor;
#endif

	vec3	diffuseColor;
#if HAS_DIFFUSE_MAP == 1
	diffuseColor = texture(material.diffuseMap, diffuseTexCoord).rgb;
#else
	diffuseColor = material.diffuseColor;
#endif

	// Directional Light
	vec3 dirColor = vec3(0.0);
#if DIR_LIGHT_COUNT > 0
	for (int i = 0; i < DIR_LIGHT_COUNT; i++)
		dirColor += calcDirLight(directionalLight[i], norm, ambientColor, diffuseColor);
#endif

	// Point Light
	vec3 pointColor = vec3(0.0);
#if POINT_LIGHT_COUNT > 0
	for (int i = 0; i < POINT_LIGHT_COUNT; i++)
		pointColor += calcPointLight(pointLight[i], norm, FragPos, ambientColor, diffuseColor);
#endif

	// Spot Light
	vec3 spotColor = vec3(0.0);
#if SPOT_LIGHT_COUNT > 0
	for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
		spotColor += calcSpotLight(spotLight[i], norm, FragPos, ambientColor, diffuseColor);
#endif

	// Final color
	vec3 finalColor = dirColor + pointColor + spotColor;

	FragColor = vec4(finalColor, 1.0);
}

#if DIR_LIGHT_COUNT > 0
vec3	calcDirLight(DirLight light, vec3 normal, vec3 ambientColor, vec3 diffuseColor)
{
	vec3	lightDir = normalize(-light.direction);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;

	return (ambient + diffuse);
}
#endif

#if POINT_LIGHT_COUNT > 0
vec3	calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 ambientColor, vec3 diffuseColor)
{
	vec3	lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;

	return (ambient + diffuse);
}
#endif

#if SPOT_LIGHT_COUNT > 0
vec3	calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 ambientColor, vec3 diffuseColor)
{
	vec3	lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;

	// Spotlight intensity
	float	theta = dot(lightDir, normalize(-light.direction));
	float	epsilon = light.innerCutOff - light.outerCutOff;
	float	intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;

	return (ambient + diffuse);
}
#endif
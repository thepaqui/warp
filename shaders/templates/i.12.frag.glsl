
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

// Specular component
#if HAS_SPECULAR_MAP == 1
	sampler2D	specularMap;
#else
	vec3		specularColor;
#endif

// Emission component
#if HAS_EMISSION_MAP == 1
	sampler2D	emissionMap;
#else
	vec3		emissionColor;
#endif

// Shininess
	float		shininess;
};

// Point Light structure
#if POINT_LIGHT_COUNT > 0
struct PointLight {
	vec3	position;
	vec3	ambientIntensity;
	vec3	diffuseIntensity;
	vec3	specularIntensity;
	float	attenuationConstant;
	float	attenuationLinear;
	float	attenuationQuadratic;
};

uniform PointLight[POINT_LIGHT_COUNT] pointLight;

vec3	calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor);
#endif

// Directional Light structure
#if DIR_LIGHT_COUNT > 0
struct DirLight {
	vec3 direction;
	vec3 ambientIntensity;
	vec3 diffuseIntensity;
	vec3 specularIntensity;
};

uniform DirLight[DIR_LIGHT_COUNT] directionalLight;

vec3	calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor);
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
	vec3 specularIntensity;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform SpotLight[SPOT_LIGHT_COUNT] spotLight;

vec3	calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor);
#endif

uniform Material material;

uniform vec3 camPos;

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

#if HAS_AMBIENT_MAP == 1
in vec2 ambientTexCoord;
#endif

#if HAS_DIFFUSE_MAP == 1
in vec2 diffuseTexCoord;
#endif

#if HAS_SPECULAR_MAP == 1
in vec2 specularTexCoord;
#endif

#if HAS_EMISSION_MAP == 1
in vec2 emissionTexCoord;
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

	vec3	specularColor;
#if HAS_SPECULAR_MAP == 1
	specularColor = texture(material.specularMap, specularTexCoord).rgb;
#else
	specularColor = material.specularColor;
#endif

	vec3 emissionColor;
#if HAS_EMISSION_MAP == 1
	emissionColor = texture(material.emissionMap, emissionTexCoord).rgb;
#else
	emissionColor = material.emissionColor;
#endif

	vec3 camDir = normalize(camPos - FragPos);

	// Directional Light
	vec3 dirColor = vec3(0.0);
#if DIR_LIGHT_COUNT > 0
	for (int i = 0; i < DIR_LIGHT_COUNT; i++)
		dirColor += calcDirLight(directionalLight[i], norm, camDir, ambientColor, diffuseColor, specularColor);
#endif

	// Point Light
	vec3 pointColor = vec3(0.0);
#if POINT_LIGHT_COUNT > 0
	for (int i = 0; i < POINT_LIGHT_COUNT; i++)
		pointColor += calcPointLight(pointLight[i], norm, FragPos, camDir, ambientColor, diffuseColor, specularColor);
#endif

	// Spot Light
	vec3 spotColor = vec3(0.0);
#if SPOT_LIGHT_COUNT > 0
	for (int i = 0; i < SPOT_LIGHT_COUNT; i++)
		spotColor += calcSpotLight(spotLight[i], norm, FragPos, camDir, ambientColor, diffuseColor, specularColor);
#endif

	// Final color
	vec3 finalColor = dirColor + pointColor + spotColor + emissionColor;

	FragColor = vec4(finalColor, 1.0);
}

#if DIR_LIGHT_COUNT > 0
vec3	calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor)
{
	vec3	lightDir = normalize(-light.direction);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3	halfDir = normalize(lightDir + viewDir);
	float	spec = 0.0;
	if (material.shininess > 0.0)
		spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;
	vec3	specular = light.specularIntensity * spec * specularColor;

	return (ambient + diffuse + specular);
}
#endif

#if POINT_LIGHT_COUNT > 0
vec3	calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor)
{
	vec3	lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3	halfDir = normalize(lightDir + viewDir);
	float	spec = 0.0;
	if (material.shininess > 0.0)
		spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;
	vec3	specular = light.specularIntensity * spec * specularColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
#endif

#if SPOT_LIGHT_COUNT > 0
vec3	calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor)
{
	vec3	lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float	diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3	halfDir = normalize(lightDir + viewDir);
	float	spec = 0.0;
	if (material.shininess > 0.0)
		spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);

	// Combine results
	vec3	ambient = light.ambientIntensity * ambientColor;
	vec3	diffuse = light.diffuseIntensity * diff * diffuseColor;
	vec3	specular = light.specularIntensity * spec * specularColor;

	// Attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.attenuationConstant + light.attenuationLinear * distance + light.attenuationQuadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// Spotlight intensity
	float	theta = dot(lightDir, normalize(-light.direction));
	float	epsilon = light.innerCutOff - light.outerCutOff;
	float	intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}
#endif
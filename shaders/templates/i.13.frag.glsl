
uniform sampler2D s2d_1; // texture
uniform bool b_1; // crystal mode
uniform float f_1; // blend
uniform float f_2; // texture tiling U
uniform float f_3; // texture tiling V
uniform vec3 v3_1; // base color

in vec3 FragPos;
in vec3 Normal;
in vec3 ObjPos;
in vec3 ObjNormal;

out vec4 FragColor;

vec3 getFaceColor(vec3 baseColor, int primitiveID)
{
	float hash = fract(sin(float(primitiveID) * 12.9898) * 43758.5453);
	float shade = mix(0.5, 1.0, hash);
	return baseColor * shade;
}

vec3 triplanarSample(sampler2D tex, vec3 objPos, vec3 objNormal)
{
	vec2 tiling = vec2(max(f_2, 0.0001), max(f_3, 0.0001));
	vec3 n = normalize(objNormal);
	vec3 w = max(abs(n) - vec3(0.2), vec3(0.0));
	w = pow(w, vec3(8.0));
	w /= max(w.x + w.y + w.z, 0.0001);

	vec2 uvX = vec2(objPos.z, objPos.y);
	vec2 uvY = vec2(objPos.x, objPos.z);
	vec2 uvZ = vec2(objPos.x, objPos.y);

	if (n.x < 0.0)
		uvX.x = -uvX.x;
	if (n.y < 0.0)
		uvY.x = -uvY.x;
	if (n.z < 0.0)
		uvZ.x = -uvZ.x;

	uvX *= tiling;
	uvY *= tiling;
	uvZ *= tiling;

	vec3 sampleX = texture(tex, uvX).rgb;
	vec3 sampleY = texture(tex, uvY).rgb;
	vec3 sampleZ = texture(tex, uvZ).rgb;

	return sampleX * w.x + sampleY * w.y + sampleZ * w.z;
}

vec3 getCrystalCoords(vec3 objPos, vec3 objNormal, vec3 worldNormal)
{
	float facetWave = sin(dot(objPos, vec3(7.1, 5.3, 6.7)) + dot(worldNormal, vec3(2.1, 3.7, 1.3)) * 2.5);
	float depthWave = cos(dot(objPos.yzx, vec3(9.2, 4.4, 8.1)));

	// Keep the crystal field attached to model space while adding a faceted distortion.
	float crystalScale = 2.0;
	float facetDistorsion = 0.10;
	float depthDistorsion = 0.04;
	return objPos * crystalScale + objNormal * facetWave * facetDistorsion + objNormal.yzx * depthWave * depthDistorsion;
}

vec3 crystalSample(sampler2D tex, vec3 fragPos, vec3 normal, vec3 objPos, vec3 objNormal)
{
	// Refraction-like bend: stronger when the view direction is more grazing to the face.
	vec3 pseudoView = normalize(fragPos + vec3(0.0001)); // the vec3(0.0001) prevents NaN when fragPos is (0,0,0)
	
	float bendStrength = 1.12;
	vec3 refrDir = refract(pseudoView, normal, 1.0 / bendStrength);

	float grazing = 1.0 - abs(dot(normal, pseudoView)); // 0 is head-on, 1 is grazing

	float baseDistorsion = 0.15;
	float grazeDistorsion = 0.95;
	float refrStrength = baseDistorsion + grazing * grazeDistorsion;

	vec3 distortedPos = fragPos + objPos * (0.45 + grazing * 0.55) + refrDir * refrStrength + objNormal * 0.25;
	vec3 crystalPos = getCrystalCoords(
		distortedPos,
		objNormal,
		normal
	);
	vec3 crystalSampleNormal = normalize(objNormal + refrDir * 0.75 + objNormal.yzx * 0.35);
	return triplanarSample(tex, crystalPos, crystalSampleNormal);
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 faceColor = getFaceColor(v3_1, gl_PrimitiveID);

	vec3 textureColor;
	if (b_1)
		textureColor = crystalSample(s2d_1, FragPos, norm, ObjPos, normalize(ObjNormal));
	else
		textureColor = triplanarSample(s2d_1, ObjPos, ObjNormal);

	vec3 finalColor = mix(faceColor, textureColor, f_1);

	// Basic lighting to avoid black areas
	finalColor *= 0.7 + 0.3 * (0.5 + 0.5 * dot(norm, vec3(0.0, 1.0, 1.0)));
	
	FragColor = vec4(finalColor, 1.0);
}

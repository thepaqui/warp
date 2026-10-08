# Default shaders guide
These default shaders are stored in the `./templates/` folder.  
They do not work as is, as they need to be completed by the engine by prepending the following:
- `#version 460 core`
- `#define POINT_LIGHT_COUNT < uint >`: number of point lights
- `#define DIR_LIGHT_COUNT < uint >`: number of directional lights
- `#define SPOT_LIGHT_COUNT < uint >`: number of spotlights
- `#define HAS_AMBIENT_MAP < 0 or 1 >`: are ambient maps used?
- `#define HAS_DIFFUSE_MAP < 0 or 1 >`: are diffuse maps used?
- `#define HAS_SPECULAR_MAP < 0 or 1 >`: are specular maps used?
- `#define HAS_EMISSION_MAP < 0 or 1 >`: are emission maps used?

These make it easy to compile fast branchless shaders while staying customizable and flexible.

Not all of these defines are actually used by all shaders, but it is simpler to provide all information for every shader, even if unused.

The completed shaders' source files are stored here (`./`), using a key system right in their filenames, which is detailed in `./KEYS.md`.  
Note that the key system's first part (the `<info>` part) is used for naming the templates.  
This avoids recreation, leaving only recompilation on subsequent uses.

# Standard illumination models
These are the 11 standard illumination models.  
Only models 0, 1 and 2 are currently implemented.  

## illum 0 (`ILLUM_MODEL_COLOR_ON_AMBIENT_OFF`)
Flat shading, using only ambient color/texture.  

### Uniforms
#### Vertex Shader
- `mat4 model`: transform (scale, rotation, translation matrix)
- `mat4 view`: camera data
- `mat4 projection`: projection data
#### Fragment Shader
- `material.`:
  - `[sampler2D ambientMap|vec3 ambientColor]`
- `directionalLight[id].`:
  - `vec3 direction`
  - `vec3 ambientIntensity`
- `pointLight[id].`:
  - `vec3 position`
  - `vec3 ambientIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`
- `spotLight[id].`:
  - `vec3 position`
  - `vec3 direction`
  - `float innerCutOff`
  - `float outerCutOff`
  - `vec3 ambientIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`

## illum 1 (`ILLUM_MODEL_COLOR_ON_AMBIENT_ON`)
Flat shading, using only ambient and diffuse colors/textures.  

### Uniforms
#### Vertex Shader
- `mat4 model`: transform (scale, rotation, translation matrix)
- `mat3 normalMatrix`: corrects normals in case of non-uniform model scaling
- `mat4 view`: camera data
- `mat4 projection`: projection data
#### Fragment Shader
- `material.`:
  - `[sampler2D ambientMap|vec3 ambientColor]`
  - `[sampler2D diffuseMap|vec3 diffuseColor]`
- `directionalLight[id].`:
  - `vec3 direction`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
- `pointLight[id].`:
  - `vec3 position`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`
- `spotLight[id].`:
  - `vec3 position`
  - `vec3 direction`
  - `float innerCutOff`
  - `float outerCutOff`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`

## illum 2 (`ILLUM_MODEL_HIGHLIGHT_ON`)
Smooth shading (Blinn-Phong), using ambient, diffuse and specular colors/textures.  

### Uniforms
#### Vertex Shader
- `mat4 model`: transform (scale, rotation, translation matrix)
- `mat3 normalMatrix`: corrects normals in case of non-uniform model scaling
- `mat4 view`: camera data
- `mat4 projection`: projection data
#### Fragment Shader
- `vec3 camPos`: camera position
- `material.`:
  - `[sampler2D ambientMap|vec3 ambientColor]`
  - `[sampler2D diffuseMap|vec3 diffuseColor]`
  - `[sampler2D specularMap|vec3 specularColor]`
  - `float shininess`
- `directionalLight[id].`:
  - `vec3 direction`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
- `pointLight[id].`:
  - `vec3 position`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`
- `spotLight[id].`:
  - `vec3 position`
  - `vec3 direction`
  - `float innerCutOff`
  - `float outerCutOff`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`

## illum 3 (`ILLUM_MODEL_REFLECTION_ON_RAY_TRACE_ON`)

## illum 4 (`ILLUM_MODEL_TRANSPARENCY_GLASS_ON_REFLECTION_RAY_TRACE_ON`)

## illum 5 (`ILLUM_MODEL_REFLECTION_FRESNEL_ON_RAY_TRACE_ON`)

## illum 6 (`ILLUM_MODEL_TRANSPARENCY_REFRACTION_ON_REFLECTION_FRESNEL_OFF_RAY_TRACE_ON`)

## illum 7 (`ILLUM_MODEL_TRANSPARENCY_REFRACTION_ON_REFLECTION_FRESNEL_ON_RAY_TRACE_ON`)

## illum 8 (`ILLUM_MODEL_REFLECTION_ON_RAY_TRACE_OFF`)

## illum 9 (`ILLUM_MODEL_TRANSPARENCY_GLASS_ON_REFLECTION_RAY_TRACE_OFF`)

## illum 10 (`ILLUM_MODEL_CAST_SHADOWS_ON_INVISIBLE_SURFACES`)

# Extra non-standard illumination models
These are extra non-standard illumination models that may be useful.  

## illum 11 (`ILLUM_MODEL_BASIC_LIGHTS`)
Used for rendering light sources, like lightbulbs.  
Unaffected by lighting.  

### Uniforms
#### Vertex Shader
- `mat4 model`: transform (scale, rotation, translation matrix)
- `mat4 view`: camera data
- `mat4 projection`: projection data
#### Fragment Shader
- `vec3 lightColor`: color of the light source

## illum 12 (`ILLUM_MODEL_HIGHLIGHT_ON_EMISSION_ON`)
Smooth shading (Blinn-Phong), using ambient, diffuse and specular colors/textures.  
Adds support for emission color/texture.  

### Uniforms
#### Vertex Shader
- `mat4 model`: transform (scale, rotation, translation matrix)
- `mat3 normalMatrix`: corrects normals in case of non-uniform model scaling
- `mat4 view`: camera data
- `mat4 projection`: projection data
#### Fragment Shader
- `vec3 camPos`: camera position
- `material.`:
  - `[sampler2D ambientMap|vec3 ambientColor]`
  - `[sampler2D diffuseMap|vec3 diffuseColor]`
  - `[sampler2D specularMap|vec3 specularColor]`
  - `[sampler2D emissionMap|vec3 emissionColor]`
  - `float shininess`
- `directionalLight[id].`:
  - `vec3 direction`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
- `pointLight[id].`:
  - `vec3 position`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`
- `spotLight[id].`:
  - `vec3 position`
  - `vec3 direction`
  - `float innerCutOff`
  - `float outerCutOff`
  - `vec3 ambientIntensity`
  - `vec3 diffuseIntensity`
  - `vec3 specularIntensity`
  - `float attenuationConstant`
  - `float attenuationLinear`
  - `float attenuationQuadratic`

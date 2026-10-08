# Shader options

- illum model or custom
- fragment or vertex shader
- number of dir lights
- number of point lights
- number of spotlights
- hasAmbientMap
- hasDiffuseMap
- hasSpecularMap
- hasEmissionMap

# Key

`<info>-<lights>-<maps>` where:  
- `<info>`: `[i|c].[id]`
- `<lights>`: `[nb_dl].[nb_pl].[nb_sl]` (all unsigned ints)
- `<maps>`: `[ham].[hdm].[hsm].[hem]` (all 0 or 1)

All in all:  
`[i|c].[id]-[nb_dl].[nb_pl].[nb_sl]-[ham].[hdm].[hsm].[hem]`

## Examples:

- `i.2-1.1.1-0.1.1.0.vert.glsl` :
  - `i`llumination model `2`, `vert`ex shader
  - `1` dir light, `1` point light, `1` spotlight
  - `0` ambient map, `1` diffuse map,
  - `1` specular map, `0` emission map

- `c.13-1.3.0-0.1.0.1.frag.glsl` :
  - `c`ustom shader `13`, `frag`ment shader
  - `1` dir light, `3` point lights, `0` spotlight
  - `0` ambient map, `1` diffuse map,
  - `0` specular map, `1` emission map
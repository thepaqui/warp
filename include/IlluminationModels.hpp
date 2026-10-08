/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IlluminationModels.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:33:12 by thepaqui          #+#    #+#             */
/*   Updated: 2026/04/10 18:18:00 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ILLUMINATION_MODELS_HPP
# define ILLUMINATION_MODELS_HPP

#include <map>

// Illumination models from MTL specification (0-10)
// and some basic template ones (11+)
// Only 0-2 and 11-13 are actually implemented in this project
enum IlluminationModel
{
	// Flat shading, only diffuse color
	ILLUM_MODEL_COLOR_ON_AMBIENT_OFF = 0,
	// Flat shading, ambient and diffuse color
	ILLUM_MODEL_COLOR_ON_AMBIENT_ON = 1,
	// Smooth shading (Blinn-Phong), ambient, diffuse and specular
	ILLUM_MODEL_HIGHLIGHT_ON = 2,
	ILLUM_MODEL_REFLECTION_ON_RAY_TRACE_ON = 3,
	ILLUM_MODEL_TRANSPARENCY_GLASS_ON_REFLECTION_RAY_TRACE_ON = 4,
	ILLUM_MODEL_REFLECTION_FRESNEL_ON_RAY_TRACE_ON = 5,
	ILLUM_MODEL_TRANSPARENCY_REFRACTION_ON_REFLECTION_FRESNEL_OFF_RAY_TRACE_ON = 6,
	ILLUM_MODEL_TRANSPARENCY_REFRACTION_ON_REFLECTION_FRESNEL_ON_RAY_TRACE_ON = 7,
	ILLUM_MODEL_REFLECTION_ON_RAY_TRACE_OFF = 8,
	ILLUM_MODEL_TRANSPARENCY_GLASS_ON_REFLECTION_RAY_TRACE_OFF = 9,
	ILLUM_MODEL_CAST_SHADOWS_ON_INVISIBLE_SURFACES = 10,
	// Shader for lights rendering (emission only)
	ILLUM_MODEL_BASIC_LIGHTS = 11,
	// Blinn-Phong with emission
	ILLUM_MODEL_HIGHLIGHT_ON_EMISSION_ON = 12
};

struct IlluminationModelTraits
{
	bool usesNormalMatrix = false;

	bool usesAmbient = false;
	bool usesDiffuse = false;
	bool usesSpecular = false;
	bool usesEmission = false;

	bool usesLights = false;

	bool usesLightColor = false;

	// These are for extra bool uniforms, add as needed
	bool usesB_1 = false; // b_1
	bool usesB_2 = false; // b_2
	bool usesB_3 = false; // b_3

	// These are for extra float uniforms, add as needed
	bool usesF_1 = false; // f_1
	bool usesF_2 = false; // f_2
	bool usesF_3 = false; // f_3

	// These are for extra vec3 uniforms, add as needed
	bool usesV3_1 = false; // v3_1
	bool usesV3_2 = false; // v3_2
	bool usesV3_3 = false; // v3_3

	// These are for extra sampler2D uniforms, add as needed
	bool usesS2D_1 = false; // s2d_1
	bool usesS2D_2 = false; // s2d_2
	bool usesS2D_3 = false; // s2d_3

	// NOTE: You could also add int, bool, vec4, matrices, etc. if needed
};

bool doesIlluminationModelUseNormalMatrix(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseAmbient(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseDiffuse(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseSpecular(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseEmission(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseShininess(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseCamPos(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseLights(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseLightColor(const IlluminationModel model) noexcept;

bool doesIlluminationModelUseB_1(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseB_2(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseB_3(const IlluminationModel model) noexcept;

bool doesIlluminationModelUseF_1(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseF_2(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseF_3(const IlluminationModel model) noexcept;

bool doesIlluminationModelUseV3_1(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseV3_2(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseV3_3(const IlluminationModel model) noexcept;

bool doesIlluminationModelUseS2D_1(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseS2D_2(const IlluminationModel model) noexcept;
bool doesIlluminationModelUseS2D_3(const IlluminationModel model) noexcept;

#endif
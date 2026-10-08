/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IlluminationModels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 17:47:18 by thepaqui          #+#    #+#             */
/*   Updated: 2026/04/10 18:18:02 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "IlluminationModels.hpp"

static const std::map<IlluminationModel, IlluminationModelTraits> illuminationModelTraits = {
	{ ILLUM_MODEL_COLOR_ON_AMBIENT_OFF,		{ false, true, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false } },
	{ ILLUM_MODEL_COLOR_ON_AMBIENT_ON,		{ true, true, true, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false } },
	{ ILLUM_MODEL_HIGHLIGHT_ON,				{ true, true, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false } },
	{ ILLUM_MODEL_BASIC_LIGHTS,				{ false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false } },
	{ ILLUM_MODEL_HIGHLIGHT_ON_EMISSION_ON,	{ true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false } }
};

bool doesIlluminationModelUseNormalMatrix(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesNormalMatrix);
}

bool doesIlluminationModelUseAmbient(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesAmbient);
}

bool doesIlluminationModelUseDiffuse(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesDiffuse);
}

bool doesIlluminationModelUseSpecular(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesSpecular);
}

bool doesIlluminationModelUseEmission(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesEmission);
}

bool doesIlluminationModelUseShininess(const IlluminationModel model) noexcept
{
	return (doesIlluminationModelUseSpecular(model));
}

bool doesIlluminationModelUseCamPos(const IlluminationModel model) noexcept
{
	return (doesIlluminationModelUseSpecular(model));
}

bool doesIlluminationModelUseLights(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesLights);
}

bool doesIlluminationModelUseLightColor(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesLightColor);
}

bool doesIlluminationModelUseB_1(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesB_1);
}

bool doesIlluminationModelUseB_2(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesB_2);
}

bool doesIlluminationModelUseB_3(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesB_3);
}

bool doesIlluminationModelUseF_1(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesF_1);
}

bool doesIlluminationModelUseF_2(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesF_2);
}

bool doesIlluminationModelUseF_3(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesF_3);
}

bool doesIlluminationModelUseV3_1(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesV3_1);
}

bool doesIlluminationModelUseV3_2(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesV3_2);
}

bool doesIlluminationModelUseV3_3(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesV3_3);
}

bool doesIlluminationModelUseS2D_1(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesS2D_1);
}

bool doesIlluminationModelUseS2D_2(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesS2D_2);
}

bool doesIlluminationModelUseS2D_3(const IlluminationModel model) noexcept
{
	return (illuminationModelTraits.at(model).usesS2D_3);
}

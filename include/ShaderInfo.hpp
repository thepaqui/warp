/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 15:17:57 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/03 14:28:56 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERINFO_HPP
# define SHADERINFO_HPP

class ShaderInfo;

# include "IlluminationModels.hpp"
# include "Material.hpp"
# include <cstddef>
# include <string>
# include <stdexcept>
# include <vector>
# include <iostream>
# include <compare>

enum ShaderType
{
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT
};

using ShaderKey = std::string;

class ShaderInfo
{
private :
	bool	isCustom = false;
	size_t	id = 0;
	IlluminationModel	model = ILLUM_MODEL_COLOR_ON_AMBIENT_OFF;

	unsigned int	dirLightCount = 0;
	unsigned int	pointLightCount = 0;
	unsigned int	spotlightCount = 0;

	bool	hasAmbientMap = false;
	bool	hasDiffuseMap = false;
	bool	hasSpecularMap = false;
	bool	hasEmissionMap = false;

	mutable ShaderKey	_key;
	mutable bool		_needKeyUpdate = false;

	void updateKey() const;

public :
	ShaderInfo& operator=(const ShaderInfo &other);
	std::strong_ordering operator<=>(const ShaderInfo &other) const;
	bool operator==(const ShaderInfo &other) const;
	bool doesMaterialMatch(const ShaderInfo &other) const;
	bool doesMaterialMatch(const Material &mat) const;

	ShaderInfo();
	ShaderInfo(const ShaderInfo &other);
	ShaderInfo(bool isCustomShader);
	~ShaderInfo() = default;

	void	makeCustom(size_t newId);
	void	makeInternal(IlluminationModel newModel);
	void	setDirLightCount(unsigned int count);
	void	setPointLightCount(unsigned int count);
	void	setSpotlightCount(unsigned int count);
	void	setLightCounts(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount);
	void	setHasAmbientMap(bool hasMap);
	void	setHasDiffuseMap(bool hasMap);
	void	setHasSpecularMap(bool hasMap);	
	void	setHasEmissionMap(bool hasMap);
	void	setMaps(bool ambientMap, bool diffuseMap, bool specularMap, bool emissionMap);

	bool	isCustomShader() const;
	bool	isInternalShader() const;
	size_t	getId() const;
	IlluminationModel	getIlluminationModel() const;
	unsigned int	getDirLightCount() const;
	unsigned int	getPointLightCount() const;
	unsigned int	getSpotlightCount() const;
	bool	getHasAmbientMap() const;
	bool	getHasDiffuseMap() const;
	bool	getHasSpecularMap() const;
	bool	getHasEmissionMap() const;

	void print() const;
	ShaderKey getKey() const;

	std::string	getFileName(ShaderType type) const;
	std::string	getTemplateFileName(ShaderType type) const;

	static ShaderInfo	createShaderInfoFromFilename(const std::string &filename);
};

namespace std
{
	template<>
	struct hash<ShaderInfo>
	{
		size_t operator()(const ShaderInfo &info) const
		{
			return std::hash<std::string>()(info.getKey());
		}
	};
}

#endif
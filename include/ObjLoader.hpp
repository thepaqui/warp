/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjLoader.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:22:00 by Copilot           #+#    #+#             */
/*   Updated: 2026/02/13 19:19:47 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_LOADER_HPP
# define OBJ_LOADER_HPP

# include "VBO.hpp"
# include "Transform.hpp"
# include "Material.hpp"
# include "MtlLoader.hpp"
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <unordered_map>
# include <cmath>
# include <limits>
# include <string>
# include <vector>
# include <map>
# include <set>

struct ObjMeshData {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

// A model can contain multiple named parts (groups) each with its own mesh
struct ObjModelData {
	struct Part {
		std::string name;
		std::string materialName; // name from usemtl, empty if none
		ObjMeshData mesh;
	};

	std::vector<Part> parts;
	// Parsed materials from an associated MTL file (name -> Material)
	std::map<std::string, Material> materials;
};

// Minimal OBJ parser: supports v, vt, vn, f (triangulated/quads/n-gons).
// Faces with quads/n-gons are fan-triangulated.
class ObjLoader {
public:
	static ObjModelData loadModel(const std::string &path);
};

#endif

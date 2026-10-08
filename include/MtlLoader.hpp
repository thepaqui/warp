/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MtlLoader.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:22:00 by Copilot           #+#    #+#             */
/*   Updated: 2025/12/16 19:37:57 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MTL_LOADER_HPP
# define MTL_LOADER_HPP

# include "Material.hpp"
# include <map>

void parseMTL(const std::string &mtlPath, std::map<std::string, Material> &out);

#endif

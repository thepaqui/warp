/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture2D.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 00:09:19 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/13 18:21:51 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# include "glad/glad.hpp"
# include <iostream>
# include <fstream>
# include <filesystem>
# include <cstring>
# include <algorithm>
# include <string>

enum TextureType
{
	TEXTURE_TYPE_AMBIENT,
	TEXTURE_TYPE_DIFFUSE,
	TEXTURE_TYPE_SPECULAR,
	TEXTURE_TYPE_EMISSION,
	TEXTURE_TYPE_NORMAL,
	TEXTURE_TYPE_HEIGHT,
	TEXTURE_TYPE_AMBIENT_OCCLUSION,
	TEXTURE_TYPE_ROUGHNESS,
	TEXTURE_TYPE_METALLIC
};

// Texture units for different texture types
// OpenGL guarantees at least 16 texture units for fragment shaders (0-15)
// Units layout:
// - 0-8: Fixed material texture types
// - 9: Reserved for texture setup (TEXTURE_SETUP_UNIT)
// - 10-15 and + if available: Available for custom texture types
enum TextureUnit
{
	TEXTURE_UNIT_AMBIENT = 0,
	TEXTURE_UNIT_DIFFUSE = 1,
	TEXTURE_UNIT_SPECULAR = 2,
	TEXTURE_UNIT_EMISSION = 3,
	TEXTURE_UNIT_NORMAL = 4,
	TEXTURE_UNIT_HEIGHT = 5,
	TEXTURE_UNIT_AMBIENT_OCCLUSION = 6,
	TEXTURE_UNIT_ROUGHNESS = 7,
	TEXTURE_UNIT_METALLIC = 8,
	TEXTURE_UNIT_SETUP = 9,
	TEXTURE_UNIT_CUSTOM_START = 10,  // Custom texture types can use units 10 and up
};

// 16 bits / 2 bytes / unsigned
typedef	__UINT16_TYPE__	WORD;
// 32 bits / 4 bytes / unsigned
typedef	__UINT32_TYPE__	DWORD;
// 32 bits / 4 bytes / signed
typedef	__INT32_TYPE__	SDWORD;

// This enforces tight packing of the structures
# pragma pack(push, 1)

// Size = 14 bytes
typedef struct	BMPFileHeader
{
	WORD	fileType{0};	// Always BM (0x4D42) for BMP files
	DWORD	fileSize{0};	// In bytes
	WORD	reserved1{0};	// Reserved values, can be ignored here
	WORD	reserved2{0};	// Reserved values, can be ignored here
	DWORD	dataOffset{0};	// Offset at which the pixel data starts in file
}				t_FileHeader;

// Size = 40 bytes
// This header has multiple versions, I only take BITMAPINFOHEADER
// Supported by at least Windows NT, 3.1x
typedef struct	BMPInfoHeader
{
	DWORD	size{0};			// Size of this header (in bytes) (includes color header size if present)
	SDWORD	width{0};			// Width of BMP image data in pixels
	SDWORD	height{0};			// Height of BMP image data in pixels (if positive, data is Y-flipped)
	WORD	planes{1};			// Always 1
	WORD	bpp{0};				// Bits per pixel
	DWORD	compression{0};		// 0 (uncompresed RGB) or 3 (uncompressed RGBA). Anything else is compressed or unsupported.
	DWORD	rawDataSize{0};		// Size of image's raw data in bytes (can be 0 if uncompressed)
	SDWORD	xPixelsPerMeter{0};
	SDWORD	yPixelsPerMeter{0};
	DWORD	colorsUsed{0};		// Nb of color indexes in the color table. 0 means the max number of colors allowed by bpp
	DWORD	colorsImportant{0};	// Nb of colors used for displaying the bitmap. If 0 all colors are required
}				t_InfoHeader;

// Size = 20 bytes
// This header is only present for transparent images (RGBA32bit)
// RGB24 does not use this header (if it is there, it can be ignored)
typedef struct	BMPColorHeader
{
	DWORD	redMask{0x00ff0000};		// Bit mask for the red channel
	DWORD	greenMask{0x0000ff00};		// Bit mask for the green channel
	DWORD	blueMask{0x000000ff};		// Bit mask for the blue channel
	DWORD	alphaMask{0xff000000};		// Bit mask for the alpha channel
	DWORD	colorSpaceType{0x73524742};	// Default "sRGB" (0x73524742)
}				t_ColorHeader;

# pragma pack(pop)

// Default Values:
// type = TEXTURE_TYPE_DIFFUSE
// S/T Wrap = GL_CLAMP_TO_BORDER
// Border Color = (0,0,0,1)
// Min/Mag Filters = GL_NEAREST
class Texture
{
private	:
	GLuint			_id = 0;
	TextureType		_type = TEXTURE_TYPE_DIFFUSE;

	unsigned char*	_data = NULL;
	unsigned int	_width = 0;
	unsigned int	_height = 0;
	unsigned int	_bpp = 0;
	bool			_yFlipped = false;
	bool			_debug = false;
	std::string		_path;

	GLint			_wrapS = GL_CLAMP_TO_BORDER;
	GLint			_wrapT = GL_CLAMP_TO_BORDER;
	GLint			_minFilter = GL_NEAREST;
	GLint			_magFilter = GL_NEAREST;
	GLfloat			_borderColor[4] = {0.f, 0.f, 0.f, 1.f};
	bool			_hasMipmaps = false;

	// Parsing functions

	void	parseBMPFileHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_FileHeader &fh);
	void	parseBMPInfoHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_InfoHeader &ih);
	void	parseBMPColorHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_ColorHeader &ch, const t_InfoHeader &ih);
	void	parseBMPData(const std::filesystem::path &fp, std::ifstream &bmp,
		const t_FileHeader &fh, const t_InfoHeader &ih);
	void	flipData(const size_t linelen, const int linenb);
	void	parseBMPFile(const std::filesystem::path &filePath);

	// Utility functions

	bool	doesFileExist(const std::string &filename) const
	{ std::ifstream	f(filename.c_str()); return (f.good()); };

	bool	isFileEmpty(std::ifstream &file) const
	{ return (static_cast<int>(file.peek()) == std::ifstream::traits_type::eof()); };

	std::ifstream	openIFileBin(const std::filesystem::path &filePath) const;

	// Reads n bytes from ifs into dst
	// ifs should be opened and valid beforehand
	void	readNBytes(std::ifstream &ifs, char *dst, size_t n)
	{ ifs.read(dst, n); };

	bool	issRGB(const t_ColorHeader &ch) const noexcept
	{ return (ch.colorSpaceType == 0x73524742); };

	bool	isRealsRGBBitMask(const t_ColorHeader &ch) const noexcept
	{ return (ch.redMask == 0x00ff0000 && ch.greenMask == 0x0000ff00
		&& ch.blueMask == 0x000000ff && ch.alphaMask == 0xff000000); };

	bool	isValidTexParam(const GLint param,
		const GLint *const acceptedValues,
		const size_t nbAccVals) const noexcept;

	// Debug functions

	void	printFileHeader(const t_FileHeader &fh) const;
	void	printInfoHeader(const t_InfoHeader &ih) const;
	void	printColorHeader(const t_ColorHeader &ch) const;
	void	printData(const t_InfoHeader &ih, const size_t padByt,
		const size_t datByt) const;
	void	printData(const t_InfoHeader &ih) const;

public	:
	Texture(const std::filesystem::path &filePath, TextureType type);
	
	// Creates new texture with same data
	Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	
	// Transfers ownership
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	
	~Texture()
	{
		free(_data);
		if (_id)
			glDeleteTextures(1, &_id);
	};

	// Activates with provided texture unit
	void	activate(GLuint unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
	};

	// Deactivates texture unit back to 0
	static void	deactivate()
	{
		glActiveTexture(GL_TEXTURE0);
	};

	// Binds without changing texture unit (for Material class)
	void	use()
	{
		glBindTexture(GL_TEXTURE_2D, _id);
	};

	// Use with specific texture unit (for direct usage)
	// Activates with provided texture unit and binds the texture
	void	use(GLuint unit)
	{
		activate(unit);
		use();
	};

	TextureType		getType() const noexcept { return _type; };
	unsigned char	*getData() const noexcept { return _data; };
	unsigned int	getWidth() const noexcept { return _width; };
	unsigned int	getHeight() const noexcept { return _height; };
	unsigned int	getBPP() const noexcept { return _bpp; };
	const std::string& getPath() const noexcept { return _path; };

	void	setType(const TextureType type) { _type = type; };
	void	setTextureMinFilter(const GLint param);
	void	setTextureMagFilter(const GLint param);
	void	setTextureWrapS(const GLint param);
	void	setTextureWrapT(const GLint param);
	void	setTextureBorderColor(float r, float g, float b, float a = 1.0f) noexcept;

	void	genMipmap()
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glGenerateMipmap(GL_TEXTURE_2D);
		_hasMipmaps = true;
	};
};

#endif
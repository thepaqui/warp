/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture2D.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 00:17:15 by thepaqui          #+#    #+#             */
/*   Updated: 2026/02/10 14:07:21 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture2D.hpp"

// Copy constructor
Texture::Texture(const Texture& other)
{
	//std::cout << "Texture Copy Constructor Called" << std::endl; // debug
	// Create a fresh texture from the same file (reuses existing constructor)
	Texture tmp(other._path, other._type);

	// Steal the resources produced by the temporary so we have valid image data
	_id = tmp._id;
	_type = tmp._type;
	_data = tmp._data;
	_width = tmp._width;
	_height = tmp._height;
	_bpp = tmp._bpp;
	_yFlipped = tmp._yFlipped;
	_debug = tmp._debug;
	_path = tmp._path;

	// Prevent the temporary from deleting the GL resources we just took
	tmp._id = 0;
	tmp._data = nullptr;
	// Copy cached GL parameter state from source (no glGet required)
	_wrapS = other._wrapS;
	_wrapT = other._wrapT;
	_minFilter = other._minFilter;
	_magFilter = other._magFilter;
	for (int i = 0; i < 4; ++i) _borderColor[i] = other._borderColor[i];
	_hasMipmaps = other._hasMipmaps;

	if (_id != 0) {
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _borderColor);
		if (_hasMipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}
}

// Move constructor
Texture::Texture(Texture&& other) noexcept
: _id(other._id), _type(other._type), _data(other._data),
	_width(other._width), _height(other._height), _bpp(other._bpp),
	_yFlipped(other._yFlipped), _debug(other._debug), _path(std::move(other._path)),
	_wrapS(other._wrapS), _wrapT(other._wrapT), _minFilter(other._minFilter), _magFilter(other._magFilter),
	_hasMipmaps(other._hasMipmaps)
{
	//std::cout << "Texture Move Constructor Called" << std::endl; // debug
	// Clear other's data to prevent deletion
	other._id = 0;
	other._data = nullptr;

	other._wrapS = GL_CLAMP_TO_BORDER;
	other._wrapT = GL_CLAMP_TO_BORDER;
	other._minFilter = GL_NEAREST;
	other._magFilter = GL_NEAREST;
	// Copy border color
	for (int i = 0; i < 4; ++i)
	{
		_borderColor[i] = other._borderColor[i];
		other._borderColor[i] = 0.f;
	}
	other._borderColor[3] = 1.f;
	other._hasMipmaps = false;
}

// Copy assignment
Texture& Texture::operator=(const Texture& other)
{
	//std::cout << "Texture Copy Assignment Operator Called" << std::endl; // debug
	if (this != &other)
	{
		// Clean up existing resources
		if (_data)
			free(_data);
		if (_id)
			glDeleteTextures(1, &_id);
		
		// Create a temporary texture and steal its contents
		Texture temp(other._path, other._type);
		_id = temp._id;
		_type = temp._type;
		_data = temp._data;
		_width = temp._width;
		_height = temp._height;
		_bpp = temp._bpp;
		_yFlipped = temp._yFlipped;
		_debug = temp._debug;
		_path = temp._path;

		_wrapS = other._wrapS;
		_wrapT = other._wrapT;
		_minFilter = other._minFilter;
		_magFilter = other._magFilter;
		for (int i = 0; i < 4; ++i) _borderColor[i] = other._borderColor[i];
		_hasMipmaps = other._hasMipmaps;

		if (_id != 0) {
			glBindTexture(GL_TEXTURE_2D, _id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _borderColor);
			if (_hasMipmaps)
				glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Prevent temp from deleting the resources we just took
		temp._id = 0;
		temp._data = nullptr;
	}
	return *this;
}

// Move assignment
Texture& Texture::operator=(Texture&& other) noexcept
{
	//std::cout << "Texture Move Assignment Operator Called" << std::endl; // debug
	if (this != &other)
	{
		// Clean up existing resources
		if (_data)
			free(_data);
		if (_id)
			glDeleteTextures(1, &_id);
			
		// Move all data
		_id = other._id;
		_type = other._type;
		_data = other._data;
		_width = other._width;
		_height = other._height;
		_bpp = other._bpp;
		_yFlipped = other._yFlipped;
		_debug = other._debug;
		_path = std::move(other._path);
		// Move cached GL params
		_wrapS = other._wrapS;
		_wrapT = other._wrapT;
		_minFilter = other._minFilter;
		_magFilter = other._magFilter;
		for (int i = 0; i < 4; ++i) _borderColor[i] = other._borderColor[i];
		_hasMipmaps = other._hasMipmaps;

		// Clear other's data
		other._id = 0;
		other._data = nullptr;
		other._wrapS = GL_CLAMP_TO_BORDER;
		other._wrapT = GL_CLAMP_TO_BORDER;
		other._minFilter = GL_NEAREST;
		other._magFilter = GL_NEAREST;
		other._borderColor[0] = 0.f; other._borderColor[1] = 0.f; other._borderColor[2] = 0.f; other._borderColor[3] = 1.f;
		other._hasMipmaps = false;
	}
	return *this;
}

Texture::Texture(const std::filesystem::path &filePath, const TextureType type)
{
	_path = filePath.string();
	_type = type;
	parseBMPFile(filePath);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_SETUP);
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	// Wrapping
	setTextureWrapS(GL_CLAMP_TO_BORDER);
	setTextureWrapT(GL_CLAMP_TO_BORDER);
	setTextureBorderColor(0, 0, 0, 1);

	// Filtering
	setTextureMinFilter(GL_NEAREST);
	setTextureMagFilter(GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height,
					0, GL_BGRA, GL_UNSIGNED_BYTE, _data);

	free(_data);
	_data = NULL;
}

void	Texture::setTextureMinFilter(const GLint param)
{
	GLint	accepted[6] =
	{
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	if (isValidTexParam(param, accepted, 6) == false)
		throw std::invalid_argument("Bad parameter for texture minifying filter");

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	_minFilter = param;
}

void	Texture::setTextureMagFilter(const GLint param)
{
	GLint	accepted[2] =
	{
		GL_NEAREST,
		GL_LINEAR
	};

	if (isValidTexParam(param, accepted, 2) == false)
		throw std::invalid_argument("Bad parameter for texture magnifying filter");

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	_magFilter = param;
}

void	Texture::setTextureWrapS(const GLint param)
{
	GLint	accepted[5] =
	{
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT,
		GL_MIRROR_CLAMP_TO_EDGE
	};

	if (isValidTexParam(param, accepted, 5) == false)
		throw std::invalid_argument("Bad parameter for horizontal texture wrapping");

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	_wrapS = param;
}

void	Texture::setTextureWrapT(const GLint param)
{
	GLint	accepted[5] =
	{
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT,
		GL_MIRROR_CLAMP_TO_EDGE
	};

	if (isValidTexParam(param, accepted, 5) == false)
		throw std::invalid_argument("Bad parameter for vertical texture wrapping");

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	_wrapT = param;
}

// If alpha is unspecified, it's assumed to be 1
// If any argument is outside of [0,1] range, it's clamped to the [0,1] range
void	Texture::setTextureBorderColor(float r, float g, float b, float a) noexcept
{
	r = std::clamp<float>(r, 0.0f, 1.0f);
	g = std::clamp<float>(g, 0.0f, 1.0f);
	b = std::clamp<float>(b, 0.0f, 1.0f);
	a = std::clamp<float>(a, 0.0f, 1.0f);
	float	borderColor[] = { r, g, b, a };

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	_borderColor[0] = borderColor[0];
	_borderColor[1] = borderColor[1];
	_borderColor[2] = borderColor[2];
	_borderColor[3] = borderColor[3];
}

/* PRIVATE METHODS */

void	Texture::parseBMPFileHeader(const std::filesystem::path &fp,
			std::ifstream &bmp, t_FileHeader &fh)
{
	readNBytes(bmp, reinterpret_cast<char*>(&fh), sizeof(fh));
	if (bmp.eof())
	{
		std::cerr << "[TEXTURE] " << fp << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(fp.string() + " is not a bmp file");
	}
	if (_debug)
		printFileHeader(fh); // debug
	if (fh.fileType != 0x4D42)
	{
		std::cerr << "[TEXTURE] " << fp << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(fp.string() + " is not a bmp file");
	}
}

void	Texture::parseBMPInfoHeader(const std::filesystem::path &fp,
			std::ifstream &bmp, t_InfoHeader &ih)
{
	readNBytes(bmp, reinterpret_cast<char*>(&ih), sizeof(ih));
	if (bmp.eof())
	{
		std::cerr << "[TEXTURE] " << fp << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(fp.string() + " is not a bmp file");
	}
	if (_debug)
		printInfoHeader(ih); // debug
	if (ih.bpp != 24 && ih.bpp != 32)
	{
		std::cerr << "[TEXTURE] " << fp << ": PIXEL ENCODING UNSUPPORTED\nONLY RGB24 AND RGBA32 ARE SUPPORTED" << std::endl;
		throw std::runtime_error(fp.string() + ": Pixel encoding unsupported");
	}
	if (ih.height < 0)
	{
		ih.height *= -1;
		_yFlipped = true;
	}
}

void	Texture::parseBMPColorHeader(const std::filesystem::path &fp,
			std::ifstream &bmp, t_ColorHeader &ch, const t_InfoHeader &ih)
{
	if (ih.bpp == 32)
	{
		if (ih.size < (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)))
		{
			std::cerr << "[TEXTURE] " << fp << ": BAD COLOR HEADER (INEXISTANT OR WRONG FORMAT)" << std::endl;
			throw std::runtime_error(fp.string() + " has no/wrong color header");
		}
		bmp.read((char*)&ch, sizeof(ch));
		if (bmp.eof())
		{
			std::cerr << "[TEXTURE] " << fp << " IS NOT A BMP FILE" << std::endl;
			throw std::runtime_error(fp.string() + " is not a bmp file");
		}
		if (_debug)
			printColorHeader(ch);
		if (!issRGB(ch))
		{
			std::cerr << "[TEXTURE] " << fp << ": COLOR ENCODING UNSUPPORTED\nONLY sRGB IS SUPPORTED FOR RGB32 ENCODING" << std::endl;
			throw std::runtime_error(fp.string() + ": Color encoding unsupported");
		}
		if (!isRealsRGBBitMask(ch))
		{
			std::cerr << "[TEXTURE] " << fp << ": WRONG COLOR MASKS FOR sRGB ENCODING" << std::endl;
			throw std::runtime_error(fp.string() + ": Wrong color masks for sRGB");
		}
	}
}

void	Texture::parseBMPData(const std::filesystem::path &fp, std::ifstream &bmp,
			const t_FileHeader &fh, const t_InfoHeader &ih)
{
	bmp.seekg(fh.dataOffset, bmp.beg);
	size_t	nbInfoBytes = ih.width * (ih.bpp / 8);
	size_t	dataBytes = ih.width * 4 * ih.height;
	size_t	nbPaddingBytes = (4 - (nbInfoBytes % 4)) % 4;
	_data = (unsigned char*)calloc(dataBytes, sizeof(unsigned char));
	if (!_data)
	{
		std::cerr << "[TEXTURE] MEMORY ALLOCATION FAILED" << std::endl;
		throw std::bad_alloc();
	}
	if (ih.bpp == 32)
		bmp.read((char*)_data, dataBytes);
	else
	{
		char	throwaway[nbPaddingBytes];
		char	*pos = NULL;
		for (int y = 0; y < ih.height; y++)
		{
			for (int x = 0; x < ih.width; x++)
			{
				pos = (char*)(_data + (y * ih.width * 4) + (x * 4));
				bmp.read(pos, 3);
				pos[3] = '\xff';
			}
			if (nbPaddingBytes)
				bmp.read(throwaway, nbPaddingBytes);
		}
		if (bmp.eof())
		{
			free(_data);
			_data = NULL;
			std::cerr << "[TEXTURE] " << fp << " HAS BAD DATA PADDING" << std::endl;
			throw std::runtime_error(fp.string() + " has bad data padding");
		}
	}
	if (_debug)
		printData(ih, nbPaddingBytes, dataBytes);
}

void	Texture::flipData(const size_t linelen, const int linenb)
{
	unsigned char	*line = (unsigned char*)calloc(linelen, sizeof(unsigned char));
	if (!line)
	{
		std::cerr << "[TEXTURE] MEMORY ALLOCATION FAILED" << std::endl;
		throw std::bad_alloc();
	}
	int	y1 = 0;
	int	y2 = linenb - 1;
	while (y1 < linenb / 2)
	{
		// copy line[y1] into line
		memcpy(line, _data + (y1 * linelen), linelen);
		// copy line[y2] into line[y1]
		memcpy(_data + (y1 * linelen), _data + (y2 * linelen), linelen);
		// copy line into line[y2]
		memcpy(_data + (y2 * linelen), line, linelen);
		y1++;
		y2--;
	}
	free(line);
}

void	Texture::parseBMPFile(const std::filesystem::path &filePath)
{
	if (!doesFileExist(filePath.string()))
	{
		std::cerr << "[TEXTURE] NO ACCESS TO " << filePath << " (INEXISTANT OR FORBIDDEN)" << std::endl;
		throw std::runtime_error(filePath.string() + " inexistant or forbidden");
	}
	std::ifstream	bmp = openIFileBin(filePath);

//	_debug = true; // debug
	t_FileHeader	fileHeader;
	parseBMPFileHeader(filePath, bmp, fileHeader);

	t_InfoHeader	infoHeader;
	parseBMPInfoHeader(filePath, bmp, infoHeader);

	t_ColorHeader	colorHeader;
	parseBMPColorHeader(filePath, bmp, colorHeader, infoHeader);

	parseBMPData(filePath, bmp, fileHeader, infoHeader);
//	_debug = false; // debug

	if (_yFlipped)
		flipData(infoHeader.width * 4, infoHeader.height);

	_width = infoHeader.width;
	_height = infoHeader.height;
	_bpp = infoHeader.bpp;
}

// Opens file in binary mode
std::ifstream	Texture::openIFileBin(const std::filesystem::path &filePath) const
{
	std::ifstream	ifs;
	ifs.open(filePath, std::ifstream::ios_base::binary);
	if (!ifs.is_open() || ifs.fail())
	{
		std::cerr << "[TEXTURE] ERROR WHILE OPENING " << filePath << std::endl;
		throw std::runtime_error("Could not open " + filePath.string());
	}
	if (isFileEmpty(ifs))
	{
		std::cerr << "[TEXTURE] " << filePath << " IS EMPTY" << std::endl;
		ifs.close();
		throw std::runtime_error(filePath.string() + " is empty");
	}
	return ifs;
}

bool	Texture::isValidTexParam(const GLint param,
	const GLint *const acceptedValues,
	const size_t nbAccVals) const noexcept
{
	if (acceptedValues == NULL)
		return false;

	for (size_t i = 0; i < nbAccVals; i++)
		if (param == acceptedValues[i])
			return true;

	return false;
}

void	Texture::printFileHeader(const t_FileHeader &fh) const
{
	std::cout << std::hex
		<< "fileType =	0x"		<< fh.fileType		<< "\n"
		<< "fileSize =	0x"		<< fh.fileSize		<< "\n"
		<< "reserved1 =	0x"		<< fh.reserved1		<< "\n"
		<< "reserved2 =	0x"		<< fh.reserved2		<< "\n"
		<< "dataOffset =	0x"	<< fh.dataOffset	<< "\n"
		<< std::dec << std::endl;
}

void	Texture::printInfoHeader(const t_InfoHeader &ih) const
{
	std::cout << std::hex
		<< "size =			0x"		<< ih.size				<< "\n"
		<< "width =			0x"		<< ih.width				<< "\n"
		<< "height =		0x"		<< ih.height			<< "\n"
		<< "planes =		0x"		<< ih.planes			<< "\n"
		<< "bpp =			0x"		<< ih.bpp				<< "\n"
		<< "compression =		0x"	<< ih.compression		<< "\n"
		<< "rawDataSize =		0x"	<< ih.rawDataSize		<< "\n"
		<< "xPixelsPerMeter =	0x"	<< ih.xPixelsPerMeter	<< "\n"
		<< "yPixelsPerMeter =	0x"	<< ih.yPixelsPerMeter	<< "\n"
		<< "colorsUsed =		0x"	<< ih.colorsUsed		<< "\n"
		<< "colorsImportant =	0x"	<< ih.colorsImportant	<< "\n"
		<< std::dec << std::endl;
}

void	Texture::printColorHeader(const t_ColorHeader &ch) const
{
	const char	*CSstr = reinterpret_cast<const char*>(&ch.colorSpaceType);
	std::cout << std::hex
		<< "redMask =		0x"		<< ch.redMask			<< "\n"
		<< "greenMask =		0x"		<< ch.greenMask			<< "\n"
		<< "blueMask =		0x"		<< ch.blueMask			<< "\n"
		<< "alphaMask =		0x"		<< ch.alphaMask			<< "\n"
		<< "colorSpaceType =	0x"	<< ch.colorSpaceType
		<< " " << CSstr[0] << CSstr[1] << CSstr[2] << CSstr[3] << "\n"
		<< std::dec << std::endl;
}

void	Texture::printData(const t_InfoHeader &ih, const size_t padByt,
			const size_t datByt) const
{
	std::cout << ih.height << " lines of " << ih.width * 4 << " bytes = "
		<< datByt << " bytes" << std::endl;
	std::cout << padByt << " bytes of padding" << std::endl;
	size_t	i = 0;
	std::cout << "data =\n" << std::hex;
	for (int y = 0; y < ih.height; y++)
	{
		std::cout << "\t";
		for (int x = 0; x < 4 * ih.width; x++)
		{
			i = (y * 4 * ih.width) + x;
			std::cout << std::setfill('0') << std::setw(2) << +_data[i] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::dec << std::endl;
}

void	Texture::printData(const t_InfoHeader &ih) const
{
	size_t	i = 0;
	std::cout << "data =\n" << std::hex;
	for (int y = 0; y < ih.height; y++)
	{
		std::cout << "\t";
		for (int x = 0; x < 4 * ih.width; x++)
		{
			i = (y * 4 * ih.width) + x;
			std::cout << std::setfill('0') << std::setw(2) << +_data[i] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::dec << std::endl;
}

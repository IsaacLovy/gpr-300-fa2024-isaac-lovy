/*
*	Author: Eric Winebrenner
*/

#pragma once
#include <vector>
#include <string>
#include "external/glad.h"
#include "external/stb_image.h"

namespace ew {
	unsigned int loadTexture(const char* filePath, int format = STBI_rgb);
	unsigned int loadTexture(const char* filePath, int format, int wrapMode, int magFilter, int minFilter, bool mipmap);
	unsigned int createCubeMap(std::vector<std::string> faces);
}

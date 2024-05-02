/*
*	Author: Eric Winebrenner
*/

#pragma once
#include <vector>
#include <string>

namespace ew {
	unsigned int loadTexture(const char* filePath);
	unsigned int loadTexture(const char* filePath, int wrapMode, int magFilter, int minFilter, bool mipmap);
	unsigned int createCubeMap(std::vector<std::string> faces);
}

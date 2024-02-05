#pragma once
#include <stdio.h>
#include "../ew/external/glad.h"
#include <GLFW/glfw3.h>

namespace ilgl
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();

		void use();
		int getColorTexture();

	private:
		int width, height;
		unsigned int fbo;
		unsigned int colorBuffer;
		unsigned int depthBuffer;
		unsigned int stencilBuffer;
	};
}
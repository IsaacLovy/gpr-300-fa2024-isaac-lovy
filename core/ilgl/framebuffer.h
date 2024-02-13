#pragma once
#include <stdio.h>
#include "../ew/external/glad.h"
#include <GLFW/glfw3.h>

namespace ilgl
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(int width, int height, bool isShadow = false);
		~FrameBuffer();

		void use();
		int getColorTexture();
		int getDepthBuffer();

	private:
		int width, height;
		unsigned int fbo;
		unsigned int colorBuffer;
		unsigned int depthBuffer;
		unsigned int stencilBuffer;
	};
}
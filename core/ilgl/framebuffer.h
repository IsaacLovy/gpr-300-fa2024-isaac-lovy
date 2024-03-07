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

		void initialize();
		void finalize();
		bool checkValidity();
		void setResolution(int width, int height);
		void addAttachment(int location, unsigned int format);
		void addDepthAttachment();

		void use();
		int getColorTexture(int location = 0);
		int getDepthBuffer();
		int getFBO();

	private:
		int width, height;
		unsigned int fbo;
		unsigned int colorBuffer[8];
		unsigned int depthBuffer;
		unsigned int stencilBuffer;
	};
}
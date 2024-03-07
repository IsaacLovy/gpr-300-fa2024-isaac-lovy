#include "framebuffer.h"

ilgl::FrameBuffer::FrameBuffer()
{
	fbo = depthBuffer = stencilBuffer = -1;
	colorBuffer[0] = 0;
	colorBuffer[1] = 0;
	colorBuffer[2] = 0;
	colorBuffer[3] = 0;
	colorBuffer[4] = 0;
	colorBuffer[5] = 0;
	colorBuffer[6] = 0;
	colorBuffer[7] = 0;
}

ilgl::FrameBuffer::FrameBuffer(int width, int height, bool isShadow)
{
	fbo = colorBuffer[0] = depthBuffer = stencilBuffer = 0;

	this->width = width;
	this->height = height;

	glCreateFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (!isShadow)
	{
		glGenTextures(1, &colorBuffer[0]);
		glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffer[0], 0);
	}

	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);

	if (isShadow)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);


	//glGenTextures(1, &stencilBuffer);
	//glBindTexture(GL_TEXTURE_2D, stencilBuffer);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_STENCIL_COMPONENTS, width, height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilBuffer, 0);
}

ilgl::FrameBuffer::~FrameBuffer()
{
}

void ilgl::FrameBuffer::initialize()
{
	glCreateFramebuffers(1, &fbo);
}

void ilgl::FrameBuffer::finalize()
{
	const GLenum drawBuffers[3] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2
	};
	glDrawBuffers(3, drawBuffers);
}

bool ilgl::FrameBuffer::checkValidity()
{
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Frame Buffer Incomplete: %d", fboStatus);
		return false;
	}

	return true;
}

void ilgl::FrameBuffer::setResolution(int width, int height)
{
	this->height = height;
	this->width = width;
}

void ilgl::FrameBuffer::addAttachment(int location, unsigned int format)
{
	glGenTextures(1, &colorBuffer[location]);
	glBindTexture(GL_TEXTURE_2D, colorBuffer[location]);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + location, colorBuffer[location], 0);
}

void ilgl::FrameBuffer::addDepthAttachment()
{
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
}

void ilgl::FrameBuffer::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int ilgl::FrameBuffer::getColorTexture(int location)
{
	return colorBuffer[location];
}

int ilgl::FrameBuffer::getDepthBuffer()
{
	return depthBuffer;
}

int ilgl::FrameBuffer::getFBO()
{
	return fbo;
}

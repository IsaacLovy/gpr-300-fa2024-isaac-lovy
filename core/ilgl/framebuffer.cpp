#include "framebuffer.h"

ilgl::FrameBuffer::FrameBuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	glCreateFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffer, 0);

	glGenTextures(1, &depthBuffer);
	glad_glGenTextures(GL_TEXTURE_2D, depthBuffer);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTTURE_2D, depthBuffer, 0);
}

void ilgl::FrameBuffer::Use()
{
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Frame Buffer Incomplete: %d", fboStatus);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

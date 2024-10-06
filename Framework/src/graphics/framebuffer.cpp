#include "framebuffer.h"

namespace Brainstorm {
	template<size_t Attachments>
	FrameBuffer<Attachments>::FrameBuffer(const std::array<Attachment, Attachments>& attachments) {
        /*glGenFramebuffers(1, &this->id);
        glBindFramebuffer(GL_FRAMEBUFFER, this->id);

        for (const Attachment& attachment : attachments) {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, attachment.width, attachment.height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), GL_TEXTURE_2D, texture, 0);
            this->attachments[i] = texture;
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Logger::fatal("Could not create framebuffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
	}
}
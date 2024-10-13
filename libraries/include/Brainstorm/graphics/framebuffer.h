#pragma once
#include <GL/glew.h>
#include <vector>

#include "../graphics/texture.h"
#include "../io/logger.h"

namespace Brainstorm {
	enum class AttachmentType : GLenum {
		DEPTH, COLOR_RGBA, COLOR_RGB, NORMAL
	};

	struct Attachment {
		GLuint texture;

		AttachmentType type;
		GLint filter, clamp;

		Attachment(AttachmentType type, GLint filter, GLint clamp);
	};

	class FrameBuffer {
	private:
		GLuint id, depthId;
		GLsizei width, height;

		std::vector<Attachment> attachments;

		inline void createAttachments();
	public:
		FrameBuffer(const std::vector<Attachment>& attachments, GLsizei width, GLsizei height);
		~FrameBuffer();

		void use() const;

		static void drop();
		static void drop(GLsizei previousWidth, GLsizei previousHeight);

		void clear() const;
		void resize(GLsizei width, GLsizei height);

		GLint getTexture(size_t attachment) const;
	};
}
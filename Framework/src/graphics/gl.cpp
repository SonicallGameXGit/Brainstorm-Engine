#include "gl.h"
#include <GL/glew.h>

namespace Brainstorm {
	void GL::setClearColor(const glm::vec3& color) {
		glClearColor(color.r, color.g, color.b, 1.0f);
	}
	void GL::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void GL::setClearColor(float red, float green, float blue, float alpha) {
		glClearColor(red, green, blue, alpha);
	}

	void GL::enableDepthTest() { glEnable(GL_DEPTH_TEST); }
	void GL::disableDepthTest() { glDisable(GL_DEPTH_TEST); }
	void GL::enableBlend() { glEnable(GL_BLEND); }
	void GL::disableBlend() { glDisable(GL_BLEND); }
	void GL::enableCull() { glEnable(GL_CULL_FACE); }
	void GL::disableCull() { glDisable(GL_CULL_FACE); }

	const unsigned int GL::COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT, GL::DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT;
	void GL::clear(unsigned int attachment) { glClear(attachment); }
}
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/glew.h>

namespace Brainstorm {
	GLint Texture::FORMAT_R = GL_R;
	GLint Texture::FORMAT_RG = GL_RG;
	GLint Texture::FORMAT_RGB = GL_RGB;
	GLint Texture::FORMAT_RGBA = GL_RGBA;

	GLint Texture::FILTER_LINEAR = GL_LINEAR;
	GLint Texture::FILTER_NEAREST = GL_NEAREST;

	bool Texture::initialized = false;

	Texture::Texture(GLint id, GLsizei width, GLsizei height) {
		this->id = id;
		this->width = width;
		this->height = height;
	}
	Texture::~Texture() {
		this->destroy();
	}

	Texture Texture::loadFromFile(const char* location, int filter) {
		int width, height, channels;
		unsigned char* pixels = stbi_load(location, &width, &height, &channels, STBI_rgb_alpha);

		if (pixels == nullptr) {
			Logger::error("Could not load texture file: \"%s\"", location);
			return Texture(0, 0, 0);
		}

		int format;
		switch (channels)
		{
		case 4:
			format = Texture::FORMAT_RGBA;
			break;
		case 3:
			format = Texture::FORMAT_RGB;
			break;
		case 1:
			format = Texture::FORMAT_R;
			break;
		case 2:
			format = Texture::FORMAT_RG;
			break;
		default:
			Logger::error("Format of texture at: \"%s\" is not supported.", location);
			return Texture(0, 0, 0);
		}

		return Texture::create(pixels, width, height, format, filter);
	}
	Texture Texture::create(const unsigned char* data, GLsizei width, GLsizei height, GLint format, GLint filter) {
		GLuint texture;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		Texture::drop();
		return Texture(texture, width, height);
	}

	unsigned int Texture::getId() const {
		return this->id;
	}

	int Texture::getWidth() const {
		return this->width;
	}
	int Texture::getHeight() const {
		return this->height;
	}

	void Texture::use(unsigned char index) const {
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
	void Texture::drop() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::destroy() const {
		glDeleteTextures(1, &this->id);
	}

	void Texture::_API_init() {
		if (Texture::initialized) {
			Logger::notice("Texture API is already initialized.");
			return;
		}

		Texture::initialized = true;
		stbi_set_flip_vertically_on_load(true);
	}
}
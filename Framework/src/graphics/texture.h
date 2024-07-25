#pragma once
#include "../io/logger.h"

namespace Brainstorm {
	class BS_API Texture {
	private:
		static bool initialized;

		unsigned int id;
		int width, height;
	public:
		static int FORMAT_RGBA;
		static int FORMAT_RGB;
		static int FORMAT_RG;
		static int FORMAT_R;

		static int FILTER_LINEAR;
		static int FILTER_NEAREST;

		Texture(int id, int width, int height);
		~Texture();

		static Texture loadFromFile(const char* location, int filter = Texture::FILTER_LINEAR);
		static Texture create(const unsigned char* data, int width, int height, int format, int filter = Texture::FILTER_LINEAR);
		
		void use(unsigned char index = 0) const;
		void destroy() const;
		
		unsigned int getId() const;
		int getWidth() const;
		int getHeight() const;

		static void drop();

		static void _API_init();
	};
}
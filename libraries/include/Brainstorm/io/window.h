#pragma once

#include <glm/ext/vector_int2.hpp>
#include "../defines.h"

namespace Brainstorm {
	class BS_API Window {
	private:
		void* handle;
		char* title;

		bool destroyed;
	public:
		Window(int width, int height, const char* title);
		~Window();

		void makeCurrent() const;
		void swapBuffers() const;
		void destroy();

		bool isRunning() const;

		void setPosition(int x, int y) const;
		void setPosition(const glm::ivec2& position) const;

		void setX(int x) const;
		void setY(int y) const;
		
		glm::ivec2 getPosition() const;
		
		int getX() const;
		int getY() const;

		void setSize(int width, int height) const;
		void setSize(const glm::ivec2& size) const;

		void setWidth(int width) const;
		void setHeight(int height) const;

		glm::ivec2 getSize() const;
		
		int getWidth() const;
		int getHeight() const;

		void setTitle(const char* format, ...);
		const char* getTitle() const;

		static void enableVSync();
		static void disableVSync();
	};
}
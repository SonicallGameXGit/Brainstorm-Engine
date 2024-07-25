#pragma once
#include <vector>

#include <glm/ext/vector_int2.hpp>

#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>

#include "input.h"
#include "../defines.h"

namespace Brainstorm {
	struct BS_API ViewportBounds {
		glm::vec2 offset, scale;
	};
	struct Runnable;

	class BS_API Window {
	private:
		unsigned int* keys;
		unsigned int* buttons;

		void* handle;
		char* title;

		bool destroyed;

		unsigned int currentFrame;
		glm::vec2 lastMousePosition, mousePosition, mouseDelta, mouseScroll, mouseScrollCapture;
	public:
		ViewportBounds viewportBounds;
		std::vector<Runnable*> runnables;

		Window(int width, int height, const char* title);
		~Window();

		virtual void onUpdate();
		
		virtual void onKeyEvent(KeyCode key, KeyAction action, int mods);
		virtual void onMouseEvent(MouseButton button, ButtonAction action, int mods);
		virtual void onMouseMoveEvent(double x, double y);
		virtual void onMouseScrollEvent(double dx, double dy);

		void addRunnable(Runnable* runnable);

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

		void* getHandle();

		bool isKeyPressed(KeyCode key) const;
		bool isKeyJustPressed(KeyCode key) const;

		bool isMouseButtonPressed(MouseButton button) const;
		bool isMouseButtonJustPressed(MouseButton button) const;

		glm::vec2 getMousePosition() const;
		glm::vec2 getMouseDelta() const;
		glm::vec2 getMouseScrollDelta() const;

		float getMouseX() const;
		float getMouseY() const;

		float getMouseDx() const;
		float getMouseDy() const;

		float getMouseScrollDx() const;
		float getMouseScrollDy() const;

		void _API_update();

		void _API_keyInput(int key, int action);
		void _API_mouseButtonInput(int button, int action);

		void _API_mouseMoveInput(double x, double y);
		void _API_mouseScrollInput(double dx, double dy);
	};

	struct BS_API Runnable {
		virtual void onUpdate(Window* window);

		virtual void onKeyEvent(Window* window, KeyCode key, KeyAction action, int mods);
		virtual void onMouseEvent(Window* window, MouseButton button, ButtonAction action, int mods);
		virtual void onMouseMoveEvent(Window* window, double x, double y);
		virtual void onMouseScrollEvent(Window* window, double dx, double dy);
	};
}
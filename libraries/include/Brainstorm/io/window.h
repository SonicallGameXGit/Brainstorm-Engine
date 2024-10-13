#pragma once
#include <vector>
#include <string>

#include <glm/ext/vector_int2.hpp>

#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "logger.h"
#include "input.h"


namespace Brainstorm {
	struct ViewportBounds {
		glm::vec2 offset = glm::vec2(), scale = glm::vec2(1.0f);
	};
	struct Runnable;

	typedef void (*KeyCallback)(KeyCode key, KeyAction action, int mods);
	typedef void (*CharCallback)(unsigned int character);
	typedef void (*MouseCallback)(MouseButton button, ButtonAction action, int mods);
	typedef void (*MouseMoveCallback)(double x, double y);
	typedef void (*MouseScrollCallback)(double dx, double dy);

	class Window {
	private:
		static bool created, closed;

		static unsigned int *keys, *buttons;
		static void* handle;

		static unsigned int currentFrame;
		static glm::vec2 lastMousePosition, mousePosition, mouseDelta, mouseScroll, mouseScrollCapture;

		static KeyCallback keyCallback;
		static CharCallback charCallback;
		static MouseCallback mouseCallback;
		static MouseMoveCallback mouseMoveCallback;
		static MouseScrollCallback mouseScrollCallback;

		static void resetMouse();
	public:
		static std::vector<Runnable*> runnables;
		static ViewportBounds viewportBounds;

		static void create(int width, int height, const char* title);
		static void swapBuffers();
		static void pollEvents();
		static void close();
		
		static void setKeyCallback(const KeyCallback callback);
		static void setCharCallback(const CharCallback callback);
		static void setMouseCallback(const MouseCallback callback);
		static void setMouseMoveCallback(const MouseMoveCallback callback);
		static void setMouseScrollCallback(const MouseScrollCallback callback);

		static float getAspect();

		static void addRunnable(Runnable* runnable);
		static bool isRunning();

		static void setPosition(int x, int y);
		static void setPosition(const glm::ivec2& position);

		static void setX(int x);
		static void setY(int y);
		
		static glm::ivec2 getPosition();
		
		static int getX();
		static int getY();

		static void setSize(int width, int height);
		static void setSize(const glm::ivec2& size);

		static void setWidth(int width);
		static void setHeight(int height);

		static glm::ivec2 getSize();
		
		static int getWidth();
		static int getHeight();

		static void setTitle(const char* title);

		static void enableVSync();
		static void disableVSync();

		static void grabMouse();
		static void releaseMouse();
		static void toggleMouse();

		static bool isMouseGrabbed();

		static void* getHandle();

		static bool isKeyPressed(KeyCode key);
		static bool isKeyJustPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseButton button);
		static bool isMouseButtonJustPressed(MouseButton button);

		static glm::vec2 getMousePosition();
		static glm::vec2 getMouseDelta();
		static glm::vec2 getMouseScrollDelta();

		static float getMouseX();
		static float getMouseY();

		static float getMouseDx();
		static float getMouseDy();

		static float getMouseScrollDx();
		static float getMouseScrollDy();
	};

	struct Runnable {
		virtual void onUpdate();

		virtual void onKeyEvent(KeyCode key, KeyAction action, int mods);
		virtual void onCharEvent(char character);
		virtual void onMouseEvent(MouseButton button, ButtonAction action, int mods);
		virtual void onMouseMoveEvent(double x, double y);
		virtual void onMouseScrollEvent(double dx, double dy);
	};
}
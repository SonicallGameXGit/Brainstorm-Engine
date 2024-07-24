#include "window.h"
#include "logger.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <cstdarg>
#include <vector>

#define HWND static_cast<GLFWwindow*>(this->handle)
#define UPTR static_cast<Window*>(glfwGetWindowUserPointer(window))

namespace Brainstorm {
    constexpr size_t Window_TitleSize = 512;

	Window::Window(int width, int height, const char* title) {
        this->handle = nullptr;
        this->destroyed = false;
        this->title = new char[Window_TitleSize]();
        this->viewportBounds = { glm::vec2(), glm::vec2(1.0f) };

        this->keys = new unsigned int[static_cast<size_t>(KeyCode::LAST) + 2]();
        this->buttons = new unsigned int[static_cast<size_t>(KeyCode::LAST) + 1]();

        this->lastMousePosition = {};
        this->mousePosition = {};
        this->mouseDelta = {};
        this->mouseScroll = {};
        this->mouseScrollCapture = {};

        memcpy(this->title, title, Window_TitleSize * sizeof(char));

        if ((this->handle = glfwCreateWindow(width, height, title, nullptr, nullptr)) == nullptr) {
            Logger::error("Could not create the window. Params: width=%d, height=%d, title=\"%s\".", width, height, title);
            return;
        }

        glfwSetWindowUserPointer(HWND, this);
        glfwSetFramebufferSizeCallback(HWND, [](GLFWwindow* window, int width, int height) -> void {
            ViewportBounds bounds = UPTR->viewportBounds;
            glViewport(
                static_cast<GLint>(bounds.offset.x * width), static_cast<GLint>(bounds.offset.y * height),
                static_cast<GLint>(bounds.scale.x * width), static_cast<GLint>(bounds.scale.y * height)
            );
        });
        
        glfwSetKeyCallback(HWND, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            UPTR->_API_keyInput(key, action);
            UPTR->onKeyEvent(KeyCode(key), KeyAction(action), mods);
        });
        glfwSetMouseButtonCallback(HWND, [](GLFWwindow* window, int button, int action, int mods) -> void {
            UPTR->_API_mouseButtonInput(button, action);
            UPTR->onMouseEvent(MouseButton(button), ButtonAction(action), mods);
        });
        glfwSetCursorPosCallback(HWND, [](GLFWwindow* window, double xpos, double ypos) -> void {
            UPTR->onMouseMoveEvent(xpos, ypos);
        });
        glfwSetScrollCallback(HWND, [](GLFWwindow* window, double xoffset, double yoffset) -> void {
            UPTR->_API_mouseScrollInput(xoffset, yoffset);
            UPTR->onMouseScrollEvent(xoffset, yoffset);
        });

        glfwMakeContextCurrent(HWND);
        Logger::info("Window \"%p\" created.", this);

        if (glewInit() != GLEW_OK) {
            Logger::error("Could not initialize GLEW for window: \"%p\".", this);
            this->destroy();

            return;
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	}
    Window::~Window() {
        delete[] this->keys;
        delete[] this->buttons;

        this->destroy();
    }

    void Window::onUpdate() {}

    void Window::onKeyEvent(KeyCode key, KeyAction action, int mods) {}
    void Window::onMouseEvent(MouseButton button, ButtonAction action, int mods) {}
    void Window::onMouseMoveEvent(double x, double y) {}
    void Window::onMouseScrollEvent(double dx, double dy) {}

    bool Window::isRunning() const {
        return !glfwWindowShouldClose(HWND);
    }

    void Window::destroy() {
        if (this->destroyed) return;
        this->destroyed = true;

        glfwDestroyWindow(HWND);
        Logger::info("Window \"%p\" destroyed.", this);
    }

    void Window::setPosition(int x, int y) const {
        glfwSetWindowPos(HWND, x, y);
    }
    void Window::setPosition(const glm::ivec2& position) const {
        glfwSetWindowPos(HWND, position.x, position.y);
    }

    void Window::setX(int x) const {
        this->setPosition(x, this->getY());
    }
    void Window::setY(int y) const {
        this->setPosition(this->getX(), y);
    }

    glm::ivec2 Window::getPosition() const {
        glm::ivec2 position{};
        glfwGetWindowPos(HWND, &position.x, &position.y);

        return position;
    }

    int Window::getX() const {
        int x;
        glfwGetWindowPos(HWND, &x, nullptr);

        return x;
    }
    int Window::getY() const {
        int y;
        glfwGetWindowPos(HWND, nullptr, &y);

        return y;
    }

    void Window::setSize(int width, int height) const {
        glfwSetWindowSize(HWND, width, height);
    }
    void Window::setSize(const glm::ivec2& size) const {
        glfwSetWindowSize(HWND, size.x, size.y);
    }

    void Window::setWidth(int width) const {
        this->setSize(width, this->getHeight());
    }
    void Window::setHeight(int height) const {
        this->setSize(this->getWidth(), height);
    }

    glm::ivec2 Window::getSize() const {
        glm::ivec2 size{};
        glfwGetWindowSize(HWND, &size.x, &size.y);

        return size;
    }

    int Window::getWidth() const {
        int width;
        glfwGetWindowSize(HWND, &width, nullptr);

        return width;
    }
    int Window::getHeight() const {
        int height;
        glfwGetWindowSize(HWND, nullptr, &height);

        return height;
    }

    void Window::setTitle(const char* format, ...) {
        va_list args;
        memset(this->title, 0, Window_TitleSize * sizeof(char));

        va_start(args, format);
        vsnprintf(this->title, Window_TitleSize * sizeof(char), format, args);
        va_end(args);

        glfwSetWindowTitle(HWND, title);
    }
    const char* Window::getTitle() const {
        return static_cast<const char*>(this->title);
    }

    void* Window::getHandle() {
        return this->handle;
    }

    void Window::enableVSync() {
        glfwSwapInterval(1);
    }
    void Window::disableVSync() {
        glfwSwapInterval(0);
    }

    bool Window::isKeyPressed(KeyCode key) const {
        return this->keys[static_cast<size_t>(key) + 1] > 0;
    }
    bool Window::isKeyJustPressed(KeyCode key) const {
        return this->keys[static_cast<size_t>(key) + 1] == this->currentFrame - 1;
    }

    bool Window::isMouseButtonPressed(MouseButton button) const {
        return this->buttons[static_cast<size_t>(button)] > 0;
    }
    bool Window::isMouseButtonJustPressed(MouseButton button) const {
        return this->buttons[static_cast<size_t>(button)] == this->currentFrame - 1;
    }

    glm::vec2 Window::getMousePosition() const {
        return this->mousePosition;
    }
    glm::vec2 Window::getMouseDelta() const {
        return this->mouseDelta;
    }
    glm::vec2 Window::getMouseScrollDelta() const {
        return this->mouseScrollCapture;
    }

    float Window::getMouseX() const {
        return this->mousePosition.x;
    }
    float Window::getMouseY() const {
        return this->mousePosition.y;
    }

    float Window::getMouseDx() const {
        return this->mouseDelta.x;
    }
    float Window::getMouseDy() const {
        return this->mouseDelta.y;
    }

    float Window::getMouseScrollDx() const {
        return this->mouseScrollCapture.x;
    }
    float Window::getMouseScrollDy() const {
        return this->mouseScrollCapture.y;
    }

    void Window::_API_update() {
        this->currentFrame++;

        this->mouseScrollCapture = this->mouseScroll;
        this->mouseScroll = glm::vec2();

        double mx, my;
        glfwGetCursorPos(HWND, &mx, &my);

        this->mousePosition = glm::vec2(static_cast<float>(mx), static_cast<float>(my));
        this->mouseDelta = this->mousePosition - this->lastMousePosition;
        this->lastMousePosition = this->mousePosition;
    }

    void Window::_API_keyInput(int key, int action) {
        switch (action) {
        case GLFW_RELEASE:
            this->keys[key + 1] = 0;
            break;
        case GLFW_PRESS:
            this->keys[key + 1] = this->currentFrame;
            break;
        }
    }
    void Window::_API_mouseButtonInput(int button, int action) {
        switch (action)
        {
        case GLFW_RELEASE:
            this->buttons[button] = 0;
            break;
        case GLFW_PRESS:
            this->buttons[button] = this->currentFrame;
            break;
        default:
            break;
        }
    }
    void Window::_API_mouseScrollInput(double dx, double dy) {
        this->mouseScroll.x += static_cast<float>(dx);
        this->mouseScroll.y += static_cast<float>(dy);
    }
}
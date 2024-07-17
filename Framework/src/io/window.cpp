#include "window.h"
#include "logger.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <cstdarg>
#include <vector>

#define HWND static_cast<GLFWwindow*>(this->handle)

namespace Brainstorm {
    constexpr size_t Window_TitleSize = 512;

	Window::Window(int width, int height, const char* title) {
        this->handle = nullptr;
        this->destroyed = false;
        this->created = false;
        this->startWidth = width;
        this->startHeight = height;

        this->title = new char[Window_TitleSize]();
        memcpy(this->title, title, Window_TitleSize * sizeof(char));
	}
    Window::~Window() {
        this->destroy();
    }

    void Window::create() {
        if (this->created) {
            Logger::notice("Window is already created!");
            return;
        }

        if ((this->handle = glfwCreateWindow(this->startWidth, this->startHeight, this->title, nullptr, nullptr)) == nullptr) {
            Logger::error("Could not create the window. Params: width=%d, height=%d, title=\"%s\".", this->startWidth, this->startHeight, this->title);
            return;
        }

        glfwMakeContextCurrent(HWND);
        Logger::info("Window \"%p\" created.", this);

        if (glewInit() != GLEW_OK) {
            Logger::error("Could not initialize GLEW for window: \"%p\".", this);
            this->destroy();

            return;
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

        this->created = true;
    }

    void Window::makeCurrent() const {
        glfwMakeContextCurrent(HWND);
    }
    void Window::swapBuffers() const {
        glfwSwapBuffers(HWND);
    }

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

    void Window::enableVSync() {
        glfwSwapInterval(1);
    }
    void Window::disableVSync() {
        glfwSwapInterval(0);
    }
}
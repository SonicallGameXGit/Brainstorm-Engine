#include "window.h"
#include "stb_image.h"

#define HWND static_cast<GLFWwindow*>(Window::handle)

namespace Brainstorm {
    bool Window::created = false, Window::closed = false;

    void* Window::handle = nullptr;
    unsigned int Window::currentFrame = 1;

    unsigned int* Window::keys = new unsigned int[static_cast<size_t>(KeyCode::LAST) + 2]();
    unsigned int* Window::buttons = new unsigned int[static_cast<size_t>(KeyCode::LAST) + 2]();

    glm::vec2 Window::lastMousePosition = {}, Window::mousePosition = {}, Window::mouseDelta = {}, Window::mouseScroll = {}, Window::mouseScrollCapture = {};
    std::vector<Runnable*> Window::runnables = {};

    ViewportBounds Window::viewportBounds = {};

    KeyCallback Window::keyCallback = nullptr;
    CharCallback Window::charCallback = nullptr;
    MouseCallback Window::mouseCallback = nullptr;
    MouseMoveCallback Window::mouseMoveCallback = nullptr;
    MouseScrollCallback Window::mouseScrollCallback = nullptr;

    void Runnable::onUpdate() {}

    void Runnable::onKeyEvent(KeyCode key, KeyAction action, int mods) {}
    void Runnable::onCharEvent(char character) {}
    void Runnable::onMouseEvent(MouseButton button, ButtonAction action, int mods) {}
    void Runnable::onMouseMoveEvent(double x, double y) {}
    void Runnable::onMouseScrollEvent(double dx, double dy) {}

	void Window::create(int width, int height, const char* title) {
        if (Window::created) {
            Logger::error("Window already created!");
            return;
        }
        Window::created = true;

        if (!glfwInit()) {
            Logger::fatal("Could not initialize Brainstorm.");
            return;
        }

        stbi_set_flip_vertically_on_load(true);
        Logger::info("Brainstorm initialized.\n");

        if ((Window::handle = glfwCreateWindow(width, height, title, nullptr, nullptr)) == nullptr) {
            Logger::error("Could not create the window. Params: width=%d, height=%d, title=\"%s\".", width, height, title);
            return;
        }

        glfwSetFramebufferSizeCallback(HWND, [](GLFWwindow* window, int width, int height) -> void {
            ViewportBounds bounds = Window::viewportBounds;
            glViewport(
                static_cast<GLint>(bounds.offset.x * width), static_cast<GLint>(bounds.offset.y * height),
                static_cast<GLint>(bounds.scale.x * width), static_cast<GLint>(bounds.scale.y * height)
            );
        });
        
        glfwSetKeyCallback(HWND, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
            switch (action) {
            case GLFW_RELEASE:
                Window::keys[key + 1] = 0;
                break;
            case GLFW_PRESS:
                Window::keys[key + 1] = Window::currentFrame;
                break;
            }

            if (Window::keyCallback != nullptr) Window::keyCallback(KeyCode(key), KeyAction(action), mods);
            for (Runnable* runnable : Window::runnables) {
                runnable->onKeyEvent(KeyCode(key), KeyAction(action), mods);
            }
        });
        glfwSetCharCallback(HWND, [](GLFWwindow* window, unsigned int codepoint) -> void {
            if (Window::charCallback != nullptr) Window::charCallback(codepoint);
            for (Runnable* runnable : Window::runnables) {
                runnable->onCharEvent(codepoint);
            }
            });
        glfwSetMouseButtonCallback(HWND, [](GLFWwindow* window, int button, int action, int mods) -> void {
            switch (action)
            {
            case GLFW_RELEASE:
                Window::buttons[button] = 0;
                break;
            case GLFW_PRESS:
                Window::buttons[button] = Window::currentFrame;
                break;
            default:
                break;
            }

            if (Window::mouseCallback != nullptr) Window::mouseCallback(MouseButton(button), ButtonAction(action), mods);
            for (Runnable* runnable : Window::runnables) {
                runnable->onMouseEvent(MouseButton(button), ButtonAction(action), mods);
            }
        });
        glfwSetCursorPosCallback(HWND, [](GLFWwindow* window, double xpos, double ypos) -> void {
            if (Window::mouseMoveCallback != nullptr) Window::mouseMoveCallback(xpos, ypos);

            for (Runnable* runnable : Window::runnables) {
                runnable->onMouseMoveEvent(xpos, ypos);
            }
        });
        glfwSetScrollCallback(HWND, [](GLFWwindow* window, double xoffset, double yoffset) -> void {
            Window::mouseScroll.x += static_cast<float>(xoffset);
            Window::mouseScroll.y += static_cast<float>(yoffset);

            if (Window::mouseScrollCallback != nullptr) Window::mouseScrollCallback(xoffset, yoffset);
            for (Runnable* runnable : Window::runnables) {
                runnable->onMouseScrollEvent(xoffset, yoffset);
            }
        });

        glfwMakeContextCurrent(HWND);
        Logger::info("Window created.", HWND);

        if (glewInit() != GLEW_OK) {
            Logger::error("Could not initialize GLEW.");
            Window::close();

            return;
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	}
    void Window::close() {
        if (Window::closed) {
            Logger::error("Window already closed.");
            return;
        }
        Window::closed = true;

        delete[] Window::keys;
        delete[] Window::buttons;

        for (Runnable* runnable : Window::runnables) {
            delete runnable;
        }

        glfwDestroyWindow(HWND);
        Logger::info("Window destroyed.");
    }

    void Window::pollEvents() {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Window::currentFrame++;

        Window::mouseScrollCapture = Window::mouseScroll;
        Window::mouseScroll = glm::vec2();

        double mx, my;
        glfwGetCursorPos(HWND, &mx, &my);

        Window::mousePosition = glm::vec2(static_cast<float>(mx), static_cast<float>(my));
        Window::mouseDelta = Window::mousePosition - Window::lastMousePosition;
        Window::lastMousePosition = Window::mousePosition;

        for (Runnable* runnable : Window::runnables) {
            runnable->onUpdate();
        }
    }
    void Window::swapBuffers() {
        glfwSwapBuffers(HWND);
    }

    void Window::setKeyCallback(const KeyCallback callback) {
        Window::keyCallback = callback;
    }
    void Window::setCharCallback(const CharCallback callback) {
        Window::charCallback = callback;
    }
    void Window::setMouseCallback(const MouseCallback callback) {
        Window::mouseCallback = callback;
    }
    void Window::setMouseMoveCallback(const MouseMoveCallback callback) {
        Window::mouseMoveCallback = callback;
    }
    void Window::setMouseScrollCallback(const MouseScrollCallback callback) {
        Window::mouseScrollCallback = callback;
    }

    float Window::getAspect() {
        return static_cast<float>(Window::getWidth()) / static_cast<float>(Window::getHeight());
    }

    bool Window::isRunning() {
        return !glfwWindowShouldClose(HWND);
    }

    void Window::addRunnable(Runnable* runnable) {
        Window::runnables.push_back(runnable);
    }

    void Window::setPosition(int x, int y) {
        glfwSetWindowPos(HWND, x, y);
    }
    void Window::setPosition(const glm::ivec2& position) {
        glfwSetWindowPos(HWND, position.x, position.y);
    }

    void Window::setX(int x) {
        Window::setPosition(x, Window::getY());
    }
    void Window::setY(int y) {
        Window::setPosition(Window::getX(), y);
    }

    glm::ivec2 Window::getPosition() {
        glm::ivec2 position{};
        glfwGetWindowPos(HWND, &position.x, &position.y);

        return position;
    }

    int Window::getX() {
        int x;
        glfwGetWindowPos(HWND, &x, nullptr);

        return x;
    }
    int Window::getY() {
        int y;
        glfwGetWindowPos(HWND, nullptr, &y);

        return y;
    }

    void Window::setSize(int width, int height) {
        glfwSetWindowSize(HWND, width, height);
    }
    void Window::setSize(const glm::ivec2& size) {
        glfwSetWindowSize(HWND, size.x, size.y);
    }

    void Window::setWidth(int width) {
        Window::setSize(width, Window::getHeight());
    }
    void Window::setHeight(int height) {
        Window::setSize(Window::getWidth(), height);
    }

    glm::ivec2 Window::getSize() {
        glm::ivec2 size{};
        glfwGetWindowSize(HWND, &size.x, &size.y);

        return size;
    }

    int Window::getWidth() {
        int width;
        glfwGetWindowSize(HWND, &width, nullptr);

        return width;
    }
    int Window::getHeight() {
        int height;
        glfwGetWindowSize(HWND, nullptr, &height);

        return height;
    }

    void Window::setTitle(const char* title) {
        glfwSetWindowTitle(HWND, title);
    }

    void* Window::getHandle() {
        return Window::handle;
    }

    void Window::grabMouse() {
        glfwSetInputMode(HWND, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Window::resetMouse();
    }
    void Window::releaseMouse() {
        glfwSetInputMode(HWND, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Window::resetMouse();
    }
    void Window::toggleMouse() {
        glfwSetInputMode(HWND, GLFW_CURSOR, Window::isMouseGrabbed() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        Window::resetMouse();
    }

    bool Window::isMouseGrabbed() {
        return glfwGetInputMode(HWND, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    }

    void Window::resetMouse() {
        Window::mouseDelta = {};

        double x, y;
        glfwGetCursorPos(HWND, &x, &y);

        Window::mousePosition = glm::vec2(static_cast<float>(x), static_cast<float>(y));
        Window::lastMousePosition = Window::mousePosition;
    }

    void Window::enableVSync() {
        glfwSwapInterval(1);
    }
    void Window::disableVSync() {
        glfwSwapInterval(0);
    }

    bool Window::isKeyPressed(KeyCode key) {
        return Window::keys[static_cast<size_t>(key) + 1] > 0;
    }
    bool Window::isKeyJustPressed(KeyCode key) {
        return Window::keys[static_cast<size_t>(key) + 1] == Window::currentFrame - 1;
    }

    bool Window::isMouseButtonPressed(MouseButton button) {
        return Window::buttons[static_cast<size_t>(button)] > 0;
    }
    bool Window::isMouseButtonJustPressed(MouseButton button) {
        return Window::buttons[static_cast<size_t>(button)] == Window::currentFrame - 1;
    }

    glm::vec2 Window::getMousePosition() {
        return Window::mousePosition;
    }
    glm::vec2 Window::getMouseDelta() {
        return Window::mouseDelta;
    }
    glm::vec2 Window::getMouseScrollDelta() {
        return Window::mouseScrollCapture;
    }

    float Window::getMouseX() {
        return Window::mousePosition.x;
    }
    float Window::getMouseY() {
        return Window::mousePosition.y;
    }

    float Window::getMouseDx() {
        return Window::mouseDelta.x;
    }
    float Window::getMouseDy() {
        return Window::mouseDelta.y;
    }

    float Window::getMouseScrollDx() {
        return Window::mouseScrollCapture.x;
    }
    float Window::getMouseScrollDy() {
        return Window::mouseScrollCapture.y;
    }
}
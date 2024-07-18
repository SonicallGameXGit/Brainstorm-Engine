#include "brainstorm.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

std::vector<Brainstorm::Application*> applications;
bool initialized = false;

namespace Brainstorm {
    Application::Application(int width, int height, const char* title) : window(width, height, title) {}

    Window* Application::getWindow() {
        return &this->window;
    }

    bool Application::operator==(const Application& other) const {
        return this == &other;
    }

    void initialize() {
        if (initialized) {
            Logger::notice("Brainstorm is already initialized!");
            return;
        }

        if (!glfwInit()) {
            Brainstorm::Logger::fatal("Could not initialize Brainstorm.");
            return;
        }

        Brainstorm::Logger::info("Brainstorm initialized.");
        initialized = true;
    }
    void registerApplication(Application* application) {
        if (!initialized) {
            Logger::fatal("Could not register Application. Brainstorm is not initialized!");
            return;
        }

        applications.push_back(application);
    }
}

int Brainstorm::run() {
    if (!initialized) {
        Logger::fatal("Could not run Brainstorm. Brainstorm is not initialized!");
        return 1;
    }

    while (applications.size() > 0) {
        glfwPollEvents();

        for (Application* application : applications) {
            application->getWindow()->makeCurrent();
            application->onUpdate();
            application->getWindow()->swapBuffers();

            if (!application->getWindow()->isRunning()) {
                applications.erase(std::find(applications.begin(), applications.end(), application));
                delete application;
            }
        }
    }

    for (Application* application : applications) {
        application->getWindow()->makeCurrent();
        delete application;
    }

    applications.clear();

    glfwTerminate();
    Logger::info("Brainstorm terminated.");

    return 0;
}
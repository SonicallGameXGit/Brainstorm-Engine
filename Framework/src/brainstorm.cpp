#include "brainstorm.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
std::vector<Brainstorm::Application*> applications;

namespace Brainstorm {
    Application::Application(int width, int height, const char* title) : window(width, height, title) {
        this->startWidth = width;
        this->startHeight = height;
        this->startTitle = title;
    }
    
    void Application::onLoad() {}
    void Application::onUpdate() {}
    void Application::onDestroy() {}

    Window* Application::getWindow() {
        return &this->window;
    }

    bool Application::operator==(const Application& other) const {
        return this == &other;
    }

    void registerApplication(Application* application) {
        applications.push_back(application);
    }
}

int Brainstorm::run() {
    if (!glfwInit()) {
        Brainstorm::Logger::fatal("Could not initialize Brainstorm.");
        return 1;
    }

    Brainstorm::Logger::info("Brainstorm initialized.");

    for (Brainstorm::Application* application : applications) {
        application->getWindow()->create();
        application->onLoad();
    }

    while (applications.size() > 0) {
        glfwPollEvents();

        for (Brainstorm::Application* application : applications) {
            application->getWindow()->makeCurrent();
            application->onUpdate();
            application->getWindow()->swapBuffers();

            if (!application->getWindow()->isRunning()) {
                application->onDestroy();
                applications.erase(std::find(applications.begin(), applications.end(), application));
                
                delete application;
            }
        }
    }

    for (Brainstorm::Application* application : applications) {
        application->getWindow()->makeCurrent();
        application->onDestroy();

        delete application;
    }

    applications.clear();

    glfwTerminate();
    Brainstorm::Logger::info("Brainstorm terminated.");

    return 0;
}
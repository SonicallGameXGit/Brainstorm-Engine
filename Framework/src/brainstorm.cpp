#include "brainstorm.h"
#include <memory>
#include <csignal>
#include <fstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

std::vector<Brainstorm::Window*> windows;
std::vector<Brainstorm::Window*> windowsToLoad;

bool initialized = false;

namespace Brainstorm {
    void initialize() {
        if (initialized) {
            Logger::notice("Brainstorm is already initialized.");
            return;
        }

        if (!glfwInit()) {
            Logger::fatal("Could not initialize Brainstorm.");
            return;
        }

        Logger::info("Brainstorm initialized.\n");
    }

    void registerWindow(Window* window) {
        windowsToLoad.push_back(window);
    }
}

static void loadApps() {
    for (Brainstorm::Window* window : windowsToLoad) {
        windows.push_back(window);
    }

    windowsToLoad.clear();
}

int Brainstorm::run() {
    do {
        loadApps();
        glfwPollEvents();

        for (Window* window : windows) {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->getHandle()));

            window->_API_update();
            window->onUpdate();

            glfwMakeContextCurrent(static_cast<GLFWwindow*>(window->getHandle()));
            glfwSwapBuffers(static_cast<GLFWwindow*>(window->getHandle()));

            if (!window->isRunning()) {
                windows.erase(
                    std::find(
                        windows.begin(),
                        windows.end(),
                        window
                    )
                );

                delete window;
            }
        }
    } while (windows.size() > 0);

    for (Window* window : windows) {
        delete window;
    }
    for (Window* window : windowsToLoad) {
        delete window;
    }

    glfwTerminate();
    Logger::info("\nBrainstorm terminated.");

    return 0;
}
#include "brainstorm.h"
#include <memory>
#include <csignal>
#include <fstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

std::vector<Brainstorm::Frame*> frames;
std::vector<Brainstorm::Frame*> framesToLoad;

bool initialized = false;

namespace Brainstorm {
    Frame::Frame(int width, int height, const char* title) : window(width, height, title) {}    
    void Frame::onUpdate() {}

    Window* Frame::getWindow() {
        return &this->window;
    }

    bool Frame::operator==(const Frame& other) const {
        return this == &other;
    }

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

    void registerFrame(Frame* frame) {
        framesToLoad.push_back(frame);
    }
}

static void loadApps() {
    for (Brainstorm::Frame* frame : framesToLoad) {
        frames.push_back(frame);
    }

    framesToLoad.clear();
}

int Brainstorm::run() {
    do {
        loadApps();
        glfwPollEvents();

        for (Frame* frame : frames) {
            frame->getWindow()->makeCurrent();
            frame->onUpdate();

            frame->getWindow()->makeCurrent();
            frame->getWindow()->swapBuffers();

            if (!frame->getWindow()->isRunning()) {
                frames.erase(
                    std::find(
                        frames.begin(),
                        frames.end(),
                        frame
                    )
                );

                delete frame;
            }
        }
    } while (frames.size() > 0);

    for (Frame* frame : frames) {
        delete frame;
    }
    for (Frame* frame : framesToLoad) {
        delete frame;
    }

    glfwTerminate();
    Logger::info("\nBrainstorm terminated.");

    return 0;
}
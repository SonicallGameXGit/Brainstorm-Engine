#include <Brainstorm/brainstorm.h>
#include <glm/gtc/matrix_transform.hpp>
#include "physics.h"
#include <format>

struct Camera {
    glm::vec3 position{}, rotation{};

    glm::mat4 getMatrix() const {
        glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::translate(matrix, -this->position);

        return matrix;
    }
};
struct FlyingCamera : Camera {
private:
    glm::vec3 velocity{};
public:
    float acceleration = 8.0f, speed = 8.0f, rotateSpeed = 0.15f;

    void update(const BS::Timer& timer) {
        glm::vec3 localForce{};
        if (BS::Window::isKeyPressed(BS::KeyCode::W)) {
            localForce.x += sin(glm::radians(this->rotation.y));
            localForce.z -= cos(glm::radians(this->rotation.y));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::S)) {
            localForce.x += sin(glm::radians(this->rotation.y + 180.0f));
            localForce.z -= cos(glm::radians(this->rotation.y + 180.0f));
        }

        if (BS::Window::isKeyPressed(BS::KeyCode::D)) {
            localForce.x += sin(glm::radians(this->rotation.y + 90.0f));
            localForce.z -= cos(glm::radians(this->rotation.y + 90.0f));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::A)) {
            localForce.x += sin(glm::radians(this->rotation.y - 90.0f));
            localForce.z -= cos(glm::radians(this->rotation.y - 90.0f));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::SPACE) || BS::Window::isKeyPressed(BS::KeyCode::E)) {
            localForce.y += 1.0f;
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::LEFT_SHIFT) || BS::Window::isKeyPressed(BS::KeyCode::Q)) {
            localForce.y -= 1.0f;
        }

        float localForceLength = glm::length(localForce);
        if (localForceLength > 0.001f) {
            localForce /= localForceLength;
        }

        this->velocity = glm::mix(this->velocity, localForce, this->acceleration * timer.getDelta());
        this->position += this->velocity * this->speed * timer.getDelta();

        this->rotation.x += BS::Window::getMouseDy() * this->rotateSpeed;
        this->rotation.x = glm::clamp(this->rotation.x, -90.0f, 90.0f);

        this->rotation.y += BS::Window::getMouseDx() * this->rotateSpeed;
        this->rotation.y -= floor(this->rotation.y / 360.0f) * 360.0f;
    }
};

struct Material {
    GLuint albedoTexture = 0;
    GLuint normalTexture = 0;

    glm::vec3 color{ 1.0f };
};
struct GameObject {
    static std::vector<GameObject*> gameObjects;

    BS::Mesh* mesh = nullptr;
    Material* material = nullptr;

    glm::vec3 position, rotation, scale;

    GameObject(BS::Mesh* mesh, Material* material = nullptr, glm::vec3 position = {}, glm::vec3 rotation = {}, glm::vec3 scale = glm::vec3(1.0f));
    ~GameObject() {
        this->destroy();
    }

    glm::mat4 getMatrix() const {
        glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->position));
        matrix = glm::rotate(matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, this->scale);

        return matrix;
    }

    void destroy() {
        GameObject::gameObjects.erase(std::find(GameObject::gameObjects.begin(), GameObject::gameObjects.end(), this));
    }
};

std::vector<GameObject*> GameObject::gameObjects = {};
GameObject::GameObject(BS::Mesh* mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = mesh;
    this->material = material;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    GameObject::gameObjects.push_back(this);
}

class GameLogic : public BS::Runnable {
private:
    BS::Timer globalTimer;

    int fps = 0, fpsCapture = 0;
    float fpsTimer = 0.0f;

    const char* windowTitle;
public:
    FlyingCamera camera{};

    BS::Mesh cubeMesh = BS::Mesh(BS::VertexBuffer({
        0,0,1,
        1,0,1,
        0,1,1,
        0,1,1,
        1,0,1,
        1,1,1,

        1,0,0,
        0,0,0,
        1,1,0,
        1,1,0,
        0,0,0,
        0,1,0,

        1,0,1,
        1,0,0,
        1,1,1,
        1,1,1,
        1,0,0,
        1,1,0,

        0,0,0,
        0,0,1,
        0,1,0,
        0,1,0,
        0,0,1,
        0,1,1,

        0,1,1,
        1,1,1,
        0,1,0,
        0,1,0,
        1,1,1,
        1,1,0,

        0,0,0,
        1,0,0,
        0,0,1,
        0,0,1,
        1,0,0,
        1,0,1
    }, 3), {
        BS::VertexBuffer({
            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1,

            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1,

            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1,

            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1,

            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1,

            0,0,
            1,0,
            0,1,
            0,1,
            1,0,
            1,1
        }, 2),
        BS::VertexBuffer({
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,

            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,
            0,0,-1,

            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,
            1,0,0,

            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,
            -1,0,0,

            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,

            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0,
            0,-1,0
        }, 3)
    }, BS::Mesh::TRIANGLES);

    Material brickMaterial{
        .albedoTexture = BS::Texture::loadFromFile("assets/textures/brick.png"),
        .normalTexture = BS::Texture::loadFromFile("assets/textures/brick_n.png")
    };

    GameObject cube = GameObject(&cubeMesh, &brickMaterial, {}, { 45.0f, 45.0f, 45.0f });
    GameObject dynamicCube = GameObject(&cubeMesh, &brickMaterial, { 0.0f, 2.0f, 0.0f });

    GameLogic(const char* windowTitle) : windowTitle(windowTitle) {
        BS::Window::grabMouse();

        this->camera.speed = 4.0f;
        this->camera.position.z = 5.0f;
    }

	void onUpdate() override {
        globalTimer.update();
        
        if (BS::Window::isMouseGrabbed()) {
            this->camera.update(globalTimer);
        }

        fpsTimer += globalTimer.getRealDelta();
        fps++;

        if (fpsTimer >= 1.0f) {
            BS::Window::setTitle(std::format("{} | FPS: {}", windowTitle, fps).c_str());

            fps = 0;
            fpsTimer = 0.0f;
        }

        if (BS::Window::isKeyJustPressed(BS::KeyCode::ESCAPE)) {
            BS::Window::toggleMouse();
        }
	}
};
class GameRenderer : public BS::Runnable {
private:
    BS::ShaderProgram shaderProgram = BS::ShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl", nullptr);
public:
    const GameLogic* gameLogic;

    GameRenderer(const GameLogic* gameLogic) {
        this->gameLogic = gameLogic;
        glEnable(GL_DEPTH_TEST);
    }

	void onUpdate() override {
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(BS::Window::getWidth()) / static_cast<float>(BS::Window::getHeight()), 0.01f, 500.0f);
       
        glm::mat4 viewMatrix = this->gameLogic->camera.getMatrix();
        this->shaderProgram.setVector3("cameraPosition", this->gameLogic->camera.position);

        glm::mat4 projViewMatrix = projectionMatrix * viewMatrix;

        this->shaderProgram.setInt("colorSampler", 0);
        this->shaderProgram.setInt("normalSampler", 1);

        for (const GameObject* gameObject : GameObject::gameObjects) {
            if (gameObject->mesh == nullptr) return;

            glm::mat4 modelMatrix = gameObject->getMatrix();

            this->shaderProgram.setMatrix4("mvp", projViewMatrix * modelMatrix);
            this->shaderProgram.setMatrix4("model", modelMatrix);
            this->shaderProgram.setVector3("color", gameObject->material->color);

            BS::Texture::use(gameObject->material->albedoTexture, 0);
            BS::Texture::use(gameObject->material->normalTexture, 1);

            gameObject->mesh->render();
        }
	}
};

int main() {
    BS::Window::create(1920, 1080, "MyNewGame!");
    GameLogic* gameLogic = new GameLogic("MyNewGame!");

    BS::Window::addRunnable(gameLogic);
    BS::Window::addRunnable(new GameRenderer(gameLogic));

    while (BS::Window::isRunning()) {
        BS::Window::pollEvents();
        BS::Window::swapBuffers();
    }

    BS::Window::close();
    return 0;
}
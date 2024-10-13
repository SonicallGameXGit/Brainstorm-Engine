#include <Brainstorm/brainstorm.h>
#include <format>
#include <vector>

namespace World {
    static float gravity = 24.0f;
    static float airAcceleration = 0.01f, groundAcceleration = 12.0f;

    static std::vector<BS::AABB3D> colliders = {};
}

struct Camera {
private:
    glm::vec3 velocity = glm::vec3();
    bool onGround = false;
public:
    float speed = 14.0f, rotateSpeed = 0.05f, jumpHeight = 7.0f;
    
    BS::AABB3D collider;
    glm::vec3 rotation;

    Camera(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : collider(position, scale), rotation(rotation) {}

    void applyInput(glm::vec2& wishVelocity) {
        if (BS::Window::isKeyPressed(BS::KeyCode::W)) {
            wishVelocity.x += sin(glm::radians(this->rotation.y));
            wishVelocity.y -= cos(glm::radians(this->rotation.y));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::S)) {
            wishVelocity.x += sin(glm::radians(this->rotation.y + 180.0f));
            wishVelocity.y -= cos(glm::radians(this->rotation.y + 180.0f));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::D)) {
            wishVelocity.x += sin(glm::radians(this->rotation.y + 90.0f));
            wishVelocity.y -= cos(glm::radians(this->rotation.y + 90.0f));
        }
        if (BS::Window::isKeyPressed(BS::KeyCode::A)) {
            wishVelocity.x += sin(glm::radians(this->rotation.y - 90.0f));
            wishVelocity.y -= cos(glm::radians(this->rotation.y - 90.0f));
        }

        if (BS::Window::isKeyPressed(BS::KeyCode::SPACE) && this->onGround) {
            this->velocity.y = this->jumpHeight;
            this->onGround = false;
        }

        float length = glm::length(wishVelocity);
        if (length >= 0.001f) {
            wishVelocity /= length;
        }
    }

    void update(const BS::Timer& timer) {
        this->rotation.x += BS::Window::getMouseDy() * this->rotateSpeed;
        this->rotation.x = glm::clamp(this->rotation.x, -90.0f, 90.0f);

        this->rotation.y += BS::Window::getMouseDx() * this->rotateSpeed;
        this->rotation.y -= floor(this->rotation.y / 360.0f) * 360.0f;

        this->velocity.y -= World::gravity * timer.getDelta();
        
        glm::vec2 wishVelocity = glm::vec2();
        this->applyInput(wishVelocity);

        this->velocity = BS::VelocityHelper::accelerate(this->velocity, glm::vec3(0.0f), (this->onGround ? World::groundAcceleration : World::airAcceleration) * timer.getDelta());

        float currentSpeed = glm::dot(glm::vec2(this->velocity.x, this->velocity.z), wishVelocity);
        float addSpeed = glm::clamp((this->onGround ? this->speed : this->speed * 0.1f) - currentSpeed, 0.0f, this->speed * 10.0f * timer.getDelta());

        this->velocity += glm::vec3(wishVelocity.x, 0.0f, wishVelocity.y) * addSpeed;

        glm::vec3 scaledVelocity = this->velocity * timer.getDelta();
        glm::vec3 finalVelocity = this->collider.move(scaledVelocity, World::colliders);

        if (scaledVelocity.x != finalVelocity.x) this->velocity.x = 0.0f;
        if (scaledVelocity.z != finalVelocity.z) this->velocity.z = 0.0f;

        if (scaledVelocity.y != 0.0f) this->onGround = false;
        if (scaledVelocity.y != finalVelocity.y) {
            this->velocity.y = 0.0f;

            if (scaledVelocity.y < 0.0f) {
                this->onGround = true;
            }
        }
    }

    glm::vec3 getEyePosition() const {
        return this->collider.position + glm::vec3(this->collider.scale.x * 0.5f, this->collider.scale.y - 0.15f, this->collider.scale.z * 0.5f);
    }

    glm::mat4 getMatrix() const {
        return BS::MatrixHelper::view(this->getEyePosition(), this->rotation);
    }
};

class GameLogic : public BS::Runnable {
private:
    BS::Timer globalTimer;

    int fps = 0, fpsCapture = 0;
    float fpsTimer = 0.0f;

    const char* windowTitle;
public:
    Camera camera = Camera(glm::vec3(), glm::vec3(), glm::vec3(0.2f, 1.76f, 0.2f));

    GameLogic(const char* windowTitle) : windowTitle(windowTitle) {
        BS::Window::grabMouse();

        this->camera.speed = 4.0f;
        this->camera.collider.position.z = 5.0f;

        World::colliders.push_back(BS::AABB3D(glm::vec3(-10.0f, -5.0f, -10.0f), glm::vec3(20.0f, 2.5f, 20.0f)));
        World::colliders.push_back(BS::AABB3D(glm::vec3(0.0f, -2.5f, 2.5f), glm::vec3(1.0f, 0.6f, 1.0f)));
        World::colliders.push_back(BS::AABB3D(glm::vec3(4.5f, -2.5f, 4.5f), glm::vec3(2.0f)));
    }

	void onUpdate() override {
        this->globalTimer.update();
        
        if (BS::Window::isMouseGrabbed()) {
            this->camera.update(this->globalTimer);
        }

        this->fpsTimer += this->globalTimer.getRealDelta();
        this->fps++;

        if (fpsTimer >= 1.0f) {
            BS::Window::setTitle(std::format("{} | FPS: {}", this->windowTitle, this->fps).c_str());

            this->fps = 0;
            this->fpsTimer = 0.0f;
        }

        if (BS::Window::isKeyJustPressed(BS::KeyCode::ESCAPE)) {
            BS::Window::toggleMouse();
        }
	}
};
class GameRenderer : public BS::Runnable {
private:
    BS::ShaderProgram shaderProgram = BS::ShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl", nullptr);
    BS::ShaderProgram postShaderProgram = BS::ShaderProgram("assets/shaders/post.vert", "assets/shaders/post.frag", nullptr);

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
    BS::Mesh squareMesh = BS::Mesh(BS::VertexBuffer({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, 2), {}, BS::Mesh::TRIANGLE_FAN);

    GLuint cubeAlbedoTexture = BS::Texture::loadFromFile("assets/textures/brick.png");
    GLuint cubeNormalTexture = BS::Texture::loadFromFile("assets/textures/brick_n.png");

    BS::FrameBuffer frameBuffer = BS::FrameBuffer({
        BS::Attachment(BS::AttachmentType::COLOR_RGB, BS::Texture::FILTER_LINEAR, BS::Texture::CLAMP_TO_EDGE),
        BS::Attachment(BS::AttachmentType::DEPTH, BS::Texture::FILTER_LINEAR, BS::Texture::CLAMP_TO_EDGE)
        }, BS::Window::getWidth(), BS::Window::getHeight());
public:
    const GameLogic* gameLogic;

    GameRenderer(const GameLogic* gameLogic) {
        this->gameLogic = gameLogic;
        glEnable(GL_DEPTH_TEST);
    }

    void render() {
        glm::mat4 projectionMatrix = BS::MatrixHelper::perspective(90.0f, BS::Window::getAspect(), 0.01f, 500.0f);

        glm::mat4 viewMatrix = this->gameLogic->camera.getMatrix();
        this->shaderProgram.setVector3("cameraPosition", this->gameLogic->camera.getEyePosition());

        glm::mat4 projViewMatrix = projectionMatrix * viewMatrix;

        this->shaderProgram.setInt("colorSampler", 0);
        this->shaderProgram.setInt("normalSampler", 1);

        for (const BS::AABB3D& collider : World::colliders) {
            glm::mat4 modelMatrix = BS::MatrixHelper::transform(collider.position, glm::vec3(), collider.scale);

            this->shaderProgram.setMatrix4("mvp", projViewMatrix * modelMatrix);
            this->shaderProgram.setMatrix4("model", modelMatrix);
            this->shaderProgram.setVector3("color", glm::vec3(1.0f));

            BS::Texture::use(this->cubeAlbedoTexture, 0);
            BS::Texture::use(this->cubeNormalTexture, 1);

            this->cubeMesh.render();
        }
    }
	void onUpdate() override {
        this->frameBuffer.use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->render();
        BS::FrameBuffer::drop();
        
        this->postShaderProgram.setInt("colorSampler", 0);
        this->postShaderProgram.setInt("depthSampler", 1);
        this->postShaderProgram.setVector2("resolution", glm::vec2(BS::Window::getSize()) / 8.0f);

        BS::Texture::use(this->frameBuffer.getTexture(0));
        BS::Texture::use(this->frameBuffer.getTexture(1), 1);

        this->squareMesh.render();
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
#include <Brainstorm/brainstorm.h>
#include <glm/gtc/matrix_transform.hpp>

struct MyNewWindow : BS::Window {
    MyNewWindow(int width, int height, const char* title);
};

class GameLogic : public BS::Runnable {
private:
    BS::Timer globalTimer;
public:
    glm::vec3 cubeColor{ 1.0f };
    glm::vec2 cubeRotation{};
    float cubeScale = 1.0f;

	void onUpdate(BS::Window* window) override {
        globalTimer.update();

        if (window->isMouseButtonPressed(BS::MouseButton::LEFT)) {
            window->grabMouse();
            
            cubeRotation.x += window->getMouseDy() * 0.1f;
            cubeRotation.y += window->getMouseDx() * 0.1f;
        }
        else {
            window->releaseMouse();
            cubeRotation.y += 22.5f * globalTimer.getDelta();
        }
        
        cubeRotation -= glm::floor(cubeRotation / 360.0f) * 360.0f;
        cubeScale += window->getMouseScrollDy() * 0.01f;

        if (window->isKeyJustPressed(BS::KeyCode::R)) {
            BS::registerWindow(new MyNewWindow(1920, 1080, "New Window"));
        }
	}
};
class GameRenderer : public BS::Runnable {
private:
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

    BS::ShaderProgram shaderProgram = BS::ShaderProgram()
        .setVertexShader("assets/shaders/vertex.glsl")
        .setFragmentShader("assets/shaders/fragment.glsl")
        .compile();

    BS::Texture cubeColorTexture = BS::Texture::loadFromFile("assets/textures/brick-wall_albedo.png");
    BS::Texture cubeNormalTexture = BS::Texture::loadFromFile("assets/textures/brick-wall_normal-ogl.png");
public:
    const GameLogic* gameLogic;

    GameRenderer(const GameLogic* gameLogic) {
        this->gameLogic = gameLogic;
        BGL::enableDepthTest();
    }

	void onUpdate(BS::Window* window) override {
        BGL::clear(BGL::COLOR_BUFFER_BIT | BGL::DEPTH_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setInt("colorSampler", 0);
        shaderProgram.setInt("normalSampler", 1);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight()), 0.01f, 500.0f);

        cubeMesh.use();

        glm::vec2 angle = -glm::radians(gameLogic->cubeRotation);
        glm::vec3 cameraPosition = gameLogic->cubeScale * 0.5f - glm::vec3(
            -sin(angle.y) * cos(angle.x),
            sin(angle.x),
            -cos(angle.y) * cos(angle.x)
        );

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::rotate(viewMatrix, glm::radians(gameLogic->cubeRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(gameLogic->cubeRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        viewMatrix = glm::translate(viewMatrix, -cameraPosition);
        
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(gameLogic->cubeScale));

        shaderProgram.setMatrix4("mvp", projectionMatrix * viewMatrix * modelMatrix);
        shaderProgram.setMatrix4("model", modelMatrix);
        shaderProgram.setVector3("color", gameLogic->cubeColor);
        shaderProgram.setVector3("cameraPosition", cameraPosition);

        cubeColorTexture.use();
        cubeNormalTexture.use(1);

        cubeMesh.render();
	}
};

MyNewWindow::MyNewWindow(int width, int height, const char* title) : Window(width, height, title) {
    GameLogic* gameLogic = new GameLogic();
    gameLogic->cubeColor = glm::normalize(glm::vec3(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f));

    this->addRunnable(gameLogic);
    this->addRunnable(new GameRenderer(gameLogic));
}

int main() {
	BS::initialize();
	BS::registerWindow(new MyNewWindow(1920, 1080, "MyNewGame!"));

	return BS::run();
}
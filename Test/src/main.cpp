#include <Brainstorm/brainstorm.h>
#include <Windows.h>

class MyNewWindow : public BS::Window {
public:
	MyNewWindow();
};

class MyNewLogic : public BS::Runnable {
private:
	float windowVelocity = 25.0f, time = 0.0f;
	BS::Timer globalTimer = BS::Timer();
public:
	void onUpdate(BS::Window* window) override {
		globalTimer.update();
		int windowPosition = window->getY();

		this->windowVelocity -= 1.0f;
		windowPosition -= static_cast<int>(ceil(this->windowVelocity));

		int wPosTemp = windowPosition;
		windowPosition = max(min(windowPosition, 2160 - window->getHeight()), 0);

		if (wPosTemp != windowPosition) {
			this->windowVelocity = 0.0f;
		}

		window->setX(window->getX() + static_cast<int>(window->getMouseScrollDy() * 20.0f));

		window->setY(windowPosition);

		this->time += globalTimer.getDelta();

		if (window->isKeyJustPressed(BS::KeyCode::R)) {
			BS::registerWindow(new MyNewWindow());
		}
	}

	void onKeyEvent(BS::Window* window, BS::KeyCode key, BS::KeyAction action, int mods) override {
		if (key == BS::KeyCode::SPACE && action == BS::KeyAction::PRESS) {
			windowVelocity = 30.0f / ((window->getWidth() + window->getHeight()) / 1500.0f);
			BS::Logger::debug("Space is pressed, yaaay!");
		}
	}
	void onMouseScrollEvent(BS::Window* window, double dx, double dy) {
		window->setX(window->getX() + static_cast<int>(dx) * 50);
		BS::Logger::debug("%f, %f", dx, dy);
	}

	float getTime() const {
		return this->time;
	}
};
class MyNewRenderer : public BS::Runnable {
private:
	BS::Mesh mesh = BS::Mesh({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLE_FAN);
	BS::ShaderProgram shaderProgram = BS::ShaderProgram()
		.setVertexShader("assets/shaders/test.vert")
		.setFragmentShader("assets/shaders/test.frag")
		.compile();

	BS::Texture testTexture = BS::Texture::loadFromFile("assets/textures/test.png");

	MyNewLogic* logic;
public:
	MyNewRenderer(MyNewLogic* logic) {
		mesh.addBuffer({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, 2);
		this->logic = logic;
	}

	void onUpdate(BS::Window* window) override {
		BGL::clear(BGL::COLOR_BUFFER_BIT | BGL::DEPTH_BUFFER_BIT);

		shaderProgram.use();
		shaderProgram.setFloat("time", logic->getTime());
		shaderProgram.setInt("colorSampler", 0);

		BS::Logger::debug("%f", logic->getTime());

		mesh.use();

		testTexture.use();
		mesh.render();
	}
};

MyNewWindow::MyNewWindow() : Window(960, 540, "SUPERDUPER TITLE") {
	MyNewLogic* logic = new MyNewLogic();

	this->addRunnable(logic);
	this->addRunnable(new MyNewRenderer(logic));
}

int main() {
	BS::initialize();
	BS::registerWindow(new MyNewWindow());

	return BS::run();
}
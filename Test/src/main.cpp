#include <Brainstorm/brainstorm.h>
#include <Windows.h>

class MyNewWindow : public BS::Window {
private:
	BS::Mesh mesh = BS::Mesh({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLE_FAN);
	BS::ShaderProgram shaderProgram = BS::ShaderProgram()
		.setVertexShader("assets/shaders/test.vert")
		.setFragmentShader("assets/shaders/test.frag")
		.compile();
public:
	MyNewWindow() : Window(960, 540, "SUPERDUPER TITLE") {
		mesh.addBuffer({ 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f }, 2);
	}

	float time = 0.0f;
	float windowVelocity = 25;

	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);
		
		int windowPosition = this->getY();

		windowVelocity -= 1.0f;
		windowPosition -= static_cast<int>(ceil(windowVelocity));

		int wPosTemp = windowPosition;
		windowPosition = max(min(windowPosition, 2160 - this->getHeight()), 0);
		
		if (wPosTemp != windowPosition) {
			windowVelocity = 0.0f;
		}

		this->setX(this->getX() + this->getMouseScrollDy() * 20);

		this->setY(windowPosition);

		time += 0.01f;

		shaderProgram.use();
		shaderProgram.setFloat("time", time);

		mesh.use();
		mesh.render();

		if (this->isKeyJustPressed(BS::KeyCode::R)) {
			BS::registerWindow(new MyNewWindow());
		}
	}
	void onKeyEvent(BS::KeyCode key, BS::KeyAction action, int mods) override {
		if (key == BS::KeyCode::SPACE && action == BS::KeyAction::PRESS) {
			windowVelocity = 30.0f / ((this->getWidth() + this->getHeight()) / 1500.0f);
			BS::Logger::debug("Space is pressed, yaaay!");
		}
	}
	void onMouseScrollEvent(double dx, double dy) {
		this->setX(this->getX() + static_cast<int>(dx) * 50);
		BS::Logger::debug("%f, %f", dx, dy);
	}
};

int main() {
	BS::initialize();
	BS::registerWindow(new MyNewWindow());

	return BS::run();
}
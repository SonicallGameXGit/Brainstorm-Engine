#include <Brainstorm/brainstorm.h>
#include <Windows.h>

class MyNewFrame : public BS::Frame {
private:
	BS::Mesh mesh = BS::Mesh({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLE_FAN);
	BS::ShaderProgram shaderProgram = BS::ShaderProgram()
		.setVertexShader("assets/shaders/test.vert")
		.setFragmentShader("assets/shaders/test.frag")
		.compile();
public:
	MyNewFrame() : Frame(960, 540, "SUPERDUPER TITLE") {
		mesh.addBuffer({ 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f }, 2);
	}

	float time = 0.0f;

	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);

		time += 0.01f;

		shaderProgram.use();
		shaderProgram.setFloat("time", time);

		mesh.use();
		mesh.render();

		BS::Mesh::drop();
		BS::ShaderProgram::drop();

		if (GetAsyncKeyState('R') & 0x01) {
			BS::registerFrame(new MyNewFrame());
		}
	}
};

int main() {
	BS::initialize();
	BS::registerFrame(new MyNewFrame());

	return BS::run();
}
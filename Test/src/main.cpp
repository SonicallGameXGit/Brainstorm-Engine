#include <Brainstorm/brainstorm.h>

class MyNewApplication : public BS::Application {
private:
	BS::Mesh mesh;
public:
	using Application::Application;

	void onLoad() override {
		mesh.create({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLES);
		mesh.addBuffer({ 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, 2);
	}
	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);

		mesh.use();
		mesh.render();

		BS::Mesh::drop();
	}
};

int main() {
	BS::registerApplication(new MyNewApplication(960, 540, "My New Brainstorm!"));
	return BS::run();
}
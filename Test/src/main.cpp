#include <Brainstorm/brainstorm.h>

class MyNewApplication : public BS::Application {
private:
	BS::Mesh mesh = BS::Mesh({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLES);
public:
	MyNewApplication() : Application(960, 540, "SUPERDUPER TITLE") {
		mesh.addBuffer({ 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }, 2);
	}

	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);

		mesh.use();
		mesh.render();

		BS::Mesh::drop();
	}
};
class MyNewApplication2 : public BS::Application {
private:
	BS::Mesh mesh = BS::Mesh({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 2, BS::Mesh::TRIANGLES);
public:
	void onUpdate() override {
		BGL::setClearColor(1.0f, 0.0f, 1.0f);
		BGL::clear(BGL::COLOR_BUFFER_BIT);

		mesh.use();
		mesh.render();

		BS::Mesh::drop();
	}
};

int main() {
	BS::initialize();
	BS::registerApplication(new MyNewApplication());
	BS::registerApplication(new MyNewApplication());
	BS::registerApplication(new MyNewApplication2());

	return BS::run();
}
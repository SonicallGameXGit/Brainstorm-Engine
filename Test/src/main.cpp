#include <Brainstorm/brainstorm.h>
#include <math.h>

class MyApplication : public BS::Application {
public:
	using Application::Application;

	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);
		BGL::setClearColor(1.0f, 0.0f, 1.0f);
	}
};
class MySecondApplication : public BS::Application {
public:
	using Application::Application;

	int frame = 0;
	void onUpdate() override {
		BGL::clear(BGL::COLOR_BUFFER_BIT);
		BGL::setClearColor(frame % 144 > 0, frame % 144 > 80, frame % 144 > 40);

		frame++;

		this->getWindow()->setTitle("MyTitle123 | Frames: %d", frame);
		this->getWindow()->setPosition(
			static_cast<int>(sin(frame / 100.0) * 250.0 + 250.0),
			static_cast<int>(cos(frame / 100.0) * 250.0 + 250.0)
		);
	}
};

int main() {
	Brainstorm::registerApplication(new MyApplication(1920, 1080, "asd"));
	Brainstorm::registerApplication(new MySecondApplication(1920));
	return Brainstorm::run();
};
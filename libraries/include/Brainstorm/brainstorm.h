#pragma once

#include "io/window.h"
#include "io/logger.h"
#include "graphics/gl.h"

namespace BS = Brainstorm;
typedef Brainstorm::GL BGL;

namespace Brainstorm {
	class BS_API Application {
	private:
		Window window;

		int startWidth, startHeight;
		const char* startTitle;
	public:
		Application(int width = 960, int height = 540, const char* title = "Untitled Brainstorm");

		virtual void onLoad();
		virtual void onUpdate();
		virtual void onDestroy();

		Window* getWindow();
		bool operator==(const Application& other) const;
	};

	void BS_API registerApplication(Application* application);
	int BS_API run();
}
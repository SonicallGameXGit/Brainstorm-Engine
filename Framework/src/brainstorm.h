#pragma once
#include "io/window.h"
#include "io/logger.h"

#include "graphics/gl.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"

namespace BS = Brainstorm;
typedef Brainstorm::GL BGL;

namespace Brainstorm {
	class BS_API Frame {
	private:
		Window window;
	public:
		Frame(int width = 960, int height = 540, const char* title = "Untitled Brainstorm");
		virtual ~Frame() = default;

		virtual void onUpdate();

		Window* getWindow();
		bool operator==(const Frame& other) const;
	};

	void BS_API initialize();
	void BS_API registerFrame(Frame* frame);
	
	int BS_API run();
}
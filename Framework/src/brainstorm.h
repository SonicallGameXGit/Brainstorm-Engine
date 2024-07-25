#pragma once
#include "io/window.h"
#include "io/logger.h"

#include "graphics/gl.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

#include "util/time.h"

namespace BS = Brainstorm;
typedef Brainstorm::GL BGL;

namespace Brainstorm {
	void BS_API initialize();
	void BS_API registerWindow(Window* window);
	
	int BS_API run();
}
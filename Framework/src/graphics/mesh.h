#pragma once
#include <vector>

#include "../defines.h"

namespace Brainstorm {
	class BS_API Mesh {
	private:
		unsigned int id;
		std::vector<unsigned int> buffers;

		size_t vertexCount;
		int renderMode;
	public:
		const static unsigned int TRIANGLES;
		const static unsigned int TRIANGLE_FAN;
		const static unsigned int TRIANGLE_STRIP;

		const static unsigned int LINES;
		const static unsigned int LINE_STRIP;
		const static unsigned int LINE_LOOP;

		const static unsigned int POINTS;

		Mesh(const std::vector<float>& vertices, int dimensions, int renderMode);
		void addBuffer(const std::vector<float>& vertices, int dimensions);
		
		void use() const;
		void render() const;

		static void drop();
	};
}
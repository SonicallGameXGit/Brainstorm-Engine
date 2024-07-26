#pragma once
#include <vector>
#include "../defines.h"

namespace Brainstorm {
	struct BS_API VertexBuffer {
		std::vector<float> data;
		int dimensions;

		VertexBuffer(const std::vector<float>& data, int dimensions);
	};

	class BS_API Mesh {
	private:
		unsigned int id;
		std::vector<unsigned int> buffers;

		unsigned int vertexCount;
		int renderMode;
	public:
		const static unsigned int TRIANGLES;
		const static unsigned int TRIANGLE_FAN;
		const static unsigned int TRIANGLE_STRIP;

		const static unsigned int LINES;
		const static unsigned int LINE_STRIP;
		const static unsigned int LINE_LOOP;

		const static unsigned int POINTS;

		Mesh(const VertexBuffer& vertices, const std::vector<VertexBuffer>& additional, int renderMode);
		~Mesh();

		void use() const;
		void render() const;
		void destroy();

		static void drop();
	};
}
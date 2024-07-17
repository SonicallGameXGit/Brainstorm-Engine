#include "mesh.h"
#include <GL/glew.h>

namespace Brainstorm {
	const unsigned int Mesh::TRIANGLES = GL_TRIANGLES;
	const unsigned int Mesh::TRIANGLE_FAN = GL_TRIANGLE_FAN;
	const unsigned int Mesh::TRIANGLE_STRIP = GL_TRIANGLE_STRIP;

	const unsigned int Mesh::LINES = GL_LINES;
	const unsigned int Mesh::LINE_STRIP = GL_LINE_STRIP;
	const unsigned int Mesh::LINE_LOOP = GL_LINE_LOOP;

	const unsigned int Mesh::POINTS = GL_POINTS;

	Mesh::Mesh() {
		this->renderMode = 0;
		this->vertexCount = 0;
		this->id = 0;
	}
	Mesh::Mesh(const std::vector<float>& vertices, int dimensions, int renderMode) {
		this->renderMode = renderMode;
		this->vertexCount = vertices.size() / dimensions;

		this->id = 0;
		glGenVertexArrays(1, &this->id);
		this->addBuffer(vertices, dimensions);

		Mesh::drop();
	}

	void Mesh::addBuffer(const std::vector<float>& vertices, int dimensions) {
		this->use();
		this->buffers.push_back(0);
		const int index = this->buffers.size() - 1;

		glGenBuffers(1, &this->buffers[index]);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[index]);

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()) * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, dimensions, GL_FLOAT, false, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void Mesh::use() const {
		glBindVertexArray(this->id);
	}
	void Mesh::render() const {
		glDrawArrays(this->renderMode, 0, this->vertexCount);
	}
	
	void Mesh::drop() {
		glBindVertexArray(0);
	}
}
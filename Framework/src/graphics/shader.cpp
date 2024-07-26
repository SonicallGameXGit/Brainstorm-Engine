#include "shader.h"
#include "../io/logger.h"

#include <sstream>
#include <fstream>
#include <GL/glew.h>

#define LOG_PROGRAM_ERROR() const size_t errorLength = 500;\
char error[errorLength];\
glGetProgramInfoLog(this->id, errorLength, nullptr, error);\
Logger::error("Could not compile ShaderProgram. Error: %s\n", error)

namespace Brainstorm {
	ShaderProgram::ShaderProgram() {
		this->id = glCreateProgram();
		this->shaders = std::array<unsigned int, 3>();
	}

	unsigned int ShaderProgram::setShader(const char* location, unsigned int type) const {
		GLuint shaderId = 0;
		std::ifstream file{ location };

		if (file.is_open()) {
			std::stringstream sstream;
			sstream << file.rdbuf();

			std::string source = sstream.str();
			const char* code = source.c_str();

			shaderId = glCreateShader(type);
			glShaderSource(shaderId, 1, &code, nullptr);
			glCompileShader(shaderId);

			GLint success;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

			if (!success) {
				std::string name = "";
				switch (type) {
				case GL_VERTEX_SHADER: {
					name = "[VERTEX SHADER]";
					break;
				}
				case GL_FRAGMENT_SHADER: {
					name = "[FRAGMENT SHADER]";
					break;
				}
				case GL_GEOMETRY_SHADER: {
					name = "[GEOMETRY SHADER]";
					break;
				}
				}

				const size_t errorLength = 500;
				
				char error[errorLength];
				glGetShaderInfoLog(shaderId, errorLength, nullptr, error);

				Logger::error("%s Could not compile! Error:\n%s\n", name.c_str(), error);
			}

			glAttachShader(this->id, shaderId);
		}

		file.close();
		return shaderId;
	}
	ShaderProgram& ShaderProgram::setVertexShader(const char* location) {
		this->shaders[0] = this->setShader(location, GL_VERTEX_SHADER);
		return *this;
	}
	ShaderProgram& ShaderProgram::setFragmentShader(const char* location) {
		this->shaders[1] = this->setShader(location, GL_FRAGMENT_SHADER);
		return *this;
	}
	ShaderProgram& ShaderProgram::setGeometryShader(const char* location) {
		this->shaders[2] = this->setShader(location, GL_GEOMETRY_SHADER);
		return *this;
	}

	ShaderProgram& ShaderProgram::compile() {
		glLinkProgram(this->id);

		GLint success;
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);

		if (!success) {
			LOG_PROGRAM_ERROR();
			return *this;
		}

		glValidateProgram(this->id);
		glGetProgramiv(this->id, GL_VALIDATE_STATUS, &success);

		if (!success) {
			LOG_PROGRAM_ERROR();
		}

		return *this;
	}

	void ShaderProgram::use() const {
		glUseProgram(this->id);
	}
	void ShaderProgram::drop() {
		glUseProgram(0);
	}

	void ShaderProgram::destroy() const {
		glDetachShader(this->id, this->shaders[0]);
		glDetachShader(this->id, this->shaders[1]);
		glDetachShader(this->id, this->shaders[2]);

		glDeleteProgram(this->id);
	}

	void ShaderProgram::setBool(const char* location, bool value) const {
		glUniform1i(glGetUniformLocation(this->id, location), value);
	}
	void ShaderProgram::setInt(const char* location, int value) const {
		glUniform1i(glGetUniformLocation(this->id, location), value);
	}
	void ShaderProgram::setFloat(const char* location, float value) const {
		glUniform1f(glGetUniformLocation(this->id, location), value);
	}
	
	void ShaderProgram::setVector2(const char* location, const glm::vec2& value) const {
		glUniform2f(glGetUniformLocation(this->id, location), value.x, value.y);
	}
	void ShaderProgram::setVector2(const char* location, float x, float y) const {
		glUniform2f(glGetUniformLocation(this->id, location), x, y);
	}
	void ShaderProgram::setVector2i(const char* location, const glm::ivec2& value) const {
		glUniform2i(glGetUniformLocation(this->id, location), value.x, value.y);
	}
	void ShaderProgram::setVector2i(const char* location, int x, int y) const {
		glUniform2i(glGetUniformLocation(this->id, location), x, y);
	}

	void ShaderProgram::setVector3(const char* location, const glm::vec3& value) const {
		glUniform3f(glGetUniformLocation(this->id, location), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector3(const char* location, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(this->id, location), x, y, z);
	}
	void ShaderProgram::setVector3i(const char* location, const glm::ivec3& value) const {
		glUniform3i(glGetUniformLocation(this->id, location), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector3i(const char* location, int x, int y, int z) const {
		glUniform3i(glGetUniformLocation(this->id, location), x, y, z);
	}

	void ShaderProgram::setVector4(const char* location, const glm::vec4& value) const {
		glUniform4f(glGetUniformLocation(this->id, location), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setVector4(const char* location, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(this->id, location), x, y, z, w);
	}
	void ShaderProgram::setVector4i(const char* location, const glm::ivec4& value) const {
		glUniform4i(glGetUniformLocation(this->id, location), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setVector4i(const char* location, int x, int y, int z, int w) const {
		glUniform4i(glGetUniformLocation(this->id, location), x, y, z, w);
	}

	void ShaderProgram::setMatrix2(const char* location, const glm::mat2& value) const {
		glUniformMatrix2fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix3(const char* location, const glm::mat3& value) const {
		glUniformMatrix3fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix4(const char* location, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
}
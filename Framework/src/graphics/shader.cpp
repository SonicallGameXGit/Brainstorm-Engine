#include "shader.h"

#define LOG_PROGRAM_ERROR() const size_t errorLength = 500;\
char error[errorLength];\
glGetProgramInfoLog(this->id, errorLength, nullptr, error);\
Logger::error("Could not compile ShaderProgram. Error: %s\n", error)

#define USE_PROGRAM() if (ShaderProgram::boundId != this->id) {\
	glUseProgram(this->id);\
	ShaderProgram::boundId = this->id;\
}

namespace Brainstorm {
	inline static void logProgramError(GLuint programId) {
		const size_t ErrorLength = 500;
		char error[ErrorLength];
		
		glGetProgramInfoLog(programId, ErrorLength, nullptr, error);
		Logger::error("Could not compile ShaderProgram. Error: %s\n", error);
	}
	inline static GLuint setShader(GLuint programId, const char* location, unsigned int type) {
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

				const size_t ErrorLength = 500;
				
				char error[ErrorLength];
				glGetShaderInfoLog(shaderId, ErrorLength, nullptr, error);

				Logger::error("%s Could not compile! Error:\n%s\n", name.c_str(), error);
			}

			glAttachShader(programId, shaderId);
		}

		file.close();
		return shaderId;
	}

	GLuint ShaderProgram::boundId = 0;

	ShaderProgram::ShaderProgram(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation) {
		this->id = glCreateProgram();
		this->shaders = std::array<unsigned int, 3>();

		if (vertexLocation != nullptr) {
			this->shaders[0] = setShader(this->id, vertexLocation, GL_VERTEX_SHADER);
		}
		if (fragmentLocation != nullptr) {
			this->shaders[1] = setShader(this->id, fragmentLocation, GL_FRAGMENT_SHADER);
		}
		if (geometryLocation != nullptr) {
			this->shaders[2] = setShader(this->id, geometryLocation, GL_GEOMETRY_SHADER);
		}

		glLinkProgram(this->id);

		GLint success;
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);

		if (!success) {
			logProgramError(this->id);
			return;
		}

		glValidateProgram(this->id);
		glGetProgramiv(this->id, GL_VALIDATE_STATUS, &success);

		if (!success) {
			logProgramError(this->id);
		}
	}

	void ShaderProgram::use() {
		USE_PROGRAM();
	}
	void ShaderProgram::drop() {
		ShaderProgram::boundId = 0;
		glUseProgram(0);
	}

	void ShaderProgram::destroy() const {
		glDetachShader(this->id, this->shaders[0]);
		glDetachShader(this->id, this->shaders[1]);
		glDetachShader(this->id, this->shaders[2]);

		glDeleteProgram(this->id);
	}

	void ShaderProgram::setBool(const char* location, bool value) const {
		USE_PROGRAM();
		glUniform1i(glGetUniformLocation(this->id, location), value);
	}
	void ShaderProgram::setInt(const char* location, int value) const {
		USE_PROGRAM();
		glUniform1i(glGetUniformLocation(this->id, location), value);
	}
	void ShaderProgram::setFloat(const char* location, float value) const {
		USE_PROGRAM();
		glUniform1f(glGetUniformLocation(this->id, location), value);
	}
	
	void ShaderProgram::setVector2(const char* location, const glm::vec2& value) const {
		USE_PROGRAM();
		glUniform2f(glGetUniformLocation(this->id, location), value.x, value.y);
	}
	void ShaderProgram::setVector2(const char* location, float x, float y) const {
		USE_PROGRAM();
		glUniform2f(glGetUniformLocation(this->id, location), x, y);
	}
	void ShaderProgram::setVector2i(const char* location, const glm::ivec2& value) const {
		USE_PROGRAM();
		glUniform2i(glGetUniformLocation(this->id, location), value.x, value.y);
	}
	void ShaderProgram::setVector2i(const char* location, int x, int y) const {
		USE_PROGRAM();
		glUniform2i(glGetUniformLocation(this->id, location), x, y);
	}

	void ShaderProgram::setVector3(const char* location, const glm::vec3& value) const {
		USE_PROGRAM();
		glUniform3f(glGetUniformLocation(this->id, location), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector3(const char* location, float x, float y, float z) const {
		USE_PROGRAM();
		glUniform3f(glGetUniformLocation(this->id, location), x, y, z);
	}
	void ShaderProgram::setVector3i(const char* location, const glm::ivec3& value) const {
		USE_PROGRAM();
		glUniform3i(glGetUniformLocation(this->id, location), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector3i(const char* location, int x, int y, int z) const {
		USE_PROGRAM();
		glUniform3i(glGetUniformLocation(this->id, location), x, y, z);
	}

	void ShaderProgram::setVector4(const char* location, const glm::vec4& value) const {
		USE_PROGRAM();
		glUniform4f(glGetUniformLocation(this->id, location), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setVector4(const char* location, float x, float y, float z, float w) const {
		USE_PROGRAM();
		glUniform4f(glGetUniformLocation(this->id, location), x, y, z, w);
	}
	void ShaderProgram::setVector4i(const char* location, const glm::ivec4& value) const {
		USE_PROGRAM();
		glUniform4i(glGetUniformLocation(this->id, location), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setVector4i(const char* location, int x, int y, int z, int w) const {
		USE_PROGRAM();
		glUniform4i(glGetUniformLocation(this->id, location), x, y, z, w);
	}

	void ShaderProgram::setMatrix2(const char* location, const glm::mat2& value) const {
		USE_PROGRAM();
		glUniformMatrix2fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix3(const char* location, const glm::mat3& value) const {
		USE_PROGRAM();
		glUniformMatrix3fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix4(const char* location, const glm::mat4& value) const {
		USE_PROGRAM();
		glUniformMatrix4fv(glGetUniformLocation(this->id, location), 1, false, &value[0][0]);
	}
}
#pragma once
#include <array>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>

#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int4.hpp>

#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "../defines.h"

namespace Brainstorm {
	class BS_API ShaderProgram {
	private:
		unsigned int id;
		std::array<unsigned int, 3> shaders;

		unsigned int setShader(const char* location, unsigned int type) const;
	public:
		ShaderProgram();

		ShaderProgram& setVertexShader(const char* location);
		ShaderProgram& setFragmentShader(const char* location);
		ShaderProgram& setGeometryShader(const char* location);

		ShaderProgram& compile();
		void destroy() const;
		
		void use() const;
		static void drop();

		void setBool(const char* location, bool value) const;
		void setInt(const char* location, int value) const;
		void setFloat(const char* location, float value) const;

		void setVector2(const char* location, const glm::vec2& value) const;
		void setVector2(const char* location, float x, float y) const;

		void setVector2i(const char* location, const glm::ivec2& value) const;
		void setVector2i(const char* location, int x, int y) const;

		void setVector3(const char* location, const glm::vec3& value) const;
		void setVector3(const char* location, float x, float y, float z) const;

		void setVector3i(const char* location, const glm::ivec3& value) const;
		void setVector3i(const char* location, int x, int y, int z) const;

		void setVector4(const char* location, const glm::vec4& value) const;
		void setVector4(const char* location, float x, float y, float z, float w) const;

		void setVector4i(const char* location, const glm::ivec4& value) const;
		void setVector4i(const char* location, int x, int y, int z, int w) const;

		void setMatrix2(const char* location, const glm::mat2& value) const;
		void setMatrix3(const char* location, const glm::mat3& value) const;
		void setMatrix4(const char* location, const glm::mat4& value) const;
	};
}
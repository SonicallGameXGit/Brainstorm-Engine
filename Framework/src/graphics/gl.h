#pragma once

#define GLM_FORCE_INLINE
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include "../defines.h"

namespace Brainstorm {
	struct BS_API GL {
		static void setClearColor(const glm::vec3& color);
		static void setClearColor(const glm::vec4& color);
		static void setClearColor(float red, float green, float blue, float alpha = 1.0f);
		static void enableDepthTest();
		static void disableDepthTest();
		static void enableBlend();
		static void disableBlend();
		static void enableCull();
		static void disableCull();

		const static unsigned int COLOR_BUFFER_BIT;
		const static unsigned int DEPTH_BUFFER_BIT;

		static void clear(unsigned int attachment);
	};
}
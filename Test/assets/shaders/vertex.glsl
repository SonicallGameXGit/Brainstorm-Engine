#version 410 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 model, mvp;

layout(location=0) out vec2 _texcoord;
layout(location=1) out vec3 _normal;
layout(location=2) out vec3 _position;

void main() {
	gl_Position = mvp * position;

	_texcoord = texcoord;
	_normal = (model * vec4(normal, 0.0)).xyz;
	_position = (model * position).xyz;
}
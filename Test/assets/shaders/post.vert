#version 410 core

layout(location=0) in vec2 position;
layout(location=0) out vec2 _texcoord;

void main() {
	gl_Position = vec4(position * 2.0 - 1.0, 0.0, 1.0);
	_texcoord = position;
}
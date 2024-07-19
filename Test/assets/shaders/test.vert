#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;

layout(location=0) out vec2 _texcoord;

void main() {
	gl_Position = position;
	_texcoord = texcoord;
}
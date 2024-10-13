#version 410 core

layout(location=0) in vec2 texcoord;

uniform sampler2D colorSampler, depthSampler;
uniform vec2 resolution;

void main() {
    gl_FragColor = mix(texture2D(colorSampler, texcoord), vec4(0.3, 0.6, 1.0, 1.0), vec4(pow(texture2D(depthSampler, texcoord).r, 128.0) / 8.0));
}
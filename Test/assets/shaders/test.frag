#version 120

varying vec2 texcoord;

uniform sampler2D colorSampler;
uniform float time;

void main() {
	gl_FragColor = texture2D(colorSampler, texcoord) * vec4(0.5 + 0.5 * cos(time + texcoord.xyx + vec3(0.0, 2.0, 4.0)), 1.0);
}
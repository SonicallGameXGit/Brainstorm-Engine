#version 410 core

const vec3 SunDirection = normalize(vec3(0.3, 1.0, 0.6));

layout(location=0) in vec2 texcoord;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 position;

uniform sampler2D colorSampler, normalSampler;
uniform vec3 color, cameraPosition;

void main() {
    vec3 mappedNormal = texture2D(normalSampler, texcoord).xyz * 2.0 - 1.0;
    mappedNormal = normalize(mix(mappedNormal, vec3(0.0, 0.0, 1.0), 0.4));

    vec3 normal = normalize(normal);
    vec3 tangent;
    vec3 bitangent;

    vec3 c1 = cross(vec3(0.0, 1.0, 0.0), normal);
    vec3 c2 = cross(vec3(1.0, 0.0, 0.0), normal);

    if (length(c1) > length(c2))
        tangent = c1;
    else
        tangent = c2;

    tangent = normalize(tangent);
    bitangent = normalize(cross(normal, tangent));

    vec3 N = normalize(normal);
    vec3 T = normalize(tangent);
    vec3 B = normalize(bitangent);
    mat3 TBN = mat3(T, B, N);

    vec3 worldNormal = normalize(TBN * mappedNormal);

    float diffuse = max(dot(worldNormal, SunDirection), 0.3);
    float specular = max(dot(reflect(SunDirection, worldNormal), normalize(position - cameraPosition)), 0.0);

	gl_FragColor = texture2D(colorSampler, texcoord);
	gl_FragColor.rgb *= color;
    gl_FragColor.rgb *= diffuse;
    gl_FragColor.rgb += pow(specular, 32.0) * 0.3;
}
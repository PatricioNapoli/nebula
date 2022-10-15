#version 400 core

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

struct PointLight {
    vec3 position;
    vec3 diffuse;
};

uniform PointLight pointLights[1];

uniform vec3 ViewPosition;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}


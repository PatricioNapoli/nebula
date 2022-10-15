#version 400 core

in vec2 MeshId;
//flat in int NumMeshes;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D[10] diffuse_textures;
uniform sampler2D texture_diffuse1;

void main() {
    FragColor = texture(diffuse_textures[int(MeshId.x)], TexCoords);
}

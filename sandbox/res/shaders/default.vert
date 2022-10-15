#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec2 meshId;
//layout (location = 4) in int numMeshes;

out vec2 TexCoords;

out vec2 MeshId;
//out int NumMeshes;

uniform mat4 mvp;

void main() {
   MeshId = meshId;
//   NumMeshes = numMeshes;

   TexCoords = texCoords;
   gl_Position = mvp * vec4(pos, 1.0);
}

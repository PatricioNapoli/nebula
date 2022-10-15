#version 330 core

layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 vp;

void main()
{
    TexCoords = position;
    vec4 pos = vp * vec4(position, 1.0);
    gl_Position = pos.xyww;
}

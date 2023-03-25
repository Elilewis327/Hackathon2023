#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mpv;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = mpv * vec4(aPos, 1.0);
}

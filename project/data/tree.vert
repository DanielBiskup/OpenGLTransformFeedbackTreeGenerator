#version 330

layout(location = 0) in vec3 position;

smooth out vec3 vertexPosition_modelSpace;

uniform mat4 MVP;

void main()
{
    vertexPosition_modelSpace = position;
    gl_Position = MVP * vec4(position, 1.0f);
}

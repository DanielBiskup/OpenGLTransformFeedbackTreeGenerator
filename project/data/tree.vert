#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in float length;

smooth out vec3 vertexPosition_modelSpace;
out float geo_length;
out vec3 geo_position;

uniform mat4 MVP;

void main()
{
    geo_position = position;
    vertexPosition_modelSpace = position;

    gl_Position = MVP * vec4(position, 1.0f);
    geo_length = length;

}

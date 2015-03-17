#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in float length;

out float geo_length;
out vec3 geo_position;

void main()
{
    geo_position = position;
    geo_length = length;
}

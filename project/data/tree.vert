#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in float length;
layout(location = 2) in vec3 normal;

out vec3 geo_position;
out float geo_length;
out vec3 geo_normal;

void main()
{
    geo_position = position;
    geo_length = length;
    geo_normal = normal;
}

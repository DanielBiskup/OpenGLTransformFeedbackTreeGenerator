#version 330

uniform vec3 oneUniform;

smooth in vec3 vertexPosition_modelSpace;

out vec4 outputColor;

void main()
{
        outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

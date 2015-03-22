#version 330

layout(location = 0) in vec3 position;
uniform mat4 MVP;
uniform mat4 MV;

smooth out vec3 frag_position;

void main()
{
    //vertexPosition_modelSpace = position;
    vec4 positionWorldSpace = (MV * vec4(position,1.0f));
    frag_position = positionWorldSpace.xyz;

    gl_Position = MVP * vec4(position, 1.0f);

}

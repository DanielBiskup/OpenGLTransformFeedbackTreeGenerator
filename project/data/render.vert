#version 330
layout(location = 0) in vec3 position_model_v;
//layout(location = 1) in vec3 normal_model_v;
//layout(location = 2) in vec2 uv_v;
uniform mat4 MVP;
uniform mat4 MV;

smooth out vec3 position_world_f;
smooth out vec3 normal_world_f;
smooth out vec2 uv_f;

void main()
{
    vec4 position_world = MV * vec4(position_model_v, 1.0f);
//    vec4 normal_world = MV * vec4(normal_model_v, 1.0f)

//    position_world_f = position_world.xyz;
//    normal_world_f = normal_world.xyz;
//    uv_f = uv_v;
   gl_Position = MVP * vec4(position_model_v, 1.0f);
}

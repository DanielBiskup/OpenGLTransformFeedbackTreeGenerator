#version 330

smooth in vec3 position_world_f;
smooth in vec3 normal_world_f;
smooth in vec2 uv_f;

out vec4 outputColor;

void main()
{
        vec3 n = normalize(normal_world_f);
        //vec3 n = normal_world_f;
        outputColor = vec4(n, 1.0f);
}

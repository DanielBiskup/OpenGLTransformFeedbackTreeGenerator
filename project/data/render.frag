#version 330

smooth in vec3 position_world_f;
smooth in vec3 normal_world_f;
smooth in vec2 uv_f;
uniform vec3 lightposition;

out vec4 outputColor;

void main()
{
        vec3 n = normalize(normal_world_f);
        vec3 p = position_world_f;
        vec3 l = normalize(lightposition-position_world_f);

        float I_diffus = dot(l, n);

        vec3 whiteLight = vec3(1.0f,1.0f,1.0f);

        //vec3 n = normal_world_f;
        outputColor = vec4(I_diffus * whiteLight, 1.0f);
}

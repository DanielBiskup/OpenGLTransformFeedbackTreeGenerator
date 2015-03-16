#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in float l[]; // Lenge die der zu generierende Ast haben soll.
//out vec3 fColor; // Output to fragment shader

void main() {

    //UNIFORMS_BEGIN:
    float scaleTriangle = 0.8f;
    float scaleLength = 0.8f;
    //UNIFORMS_END:

    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    vec3 a = p1 - p0;
    vec3 b = p2 - p0;
    vec3 n = cross(a, b);
    vec3 h = n * l[0];
    vec3 c = (p0+p1+p2)/3;

    vec3 d0 = (p0 - c) * scaleTriangle;
    vec3 d1 = (p1 - c) * scaleTriangle;
    vec3 d2 = (p2 - c) * scaleTriangle;

    vec3 q0 = c + d0 + h;
    vec3 q1 = c + d1 + h;
    vec3 q2 = c + d2 + h;

    gl_Position = vec4(p0,1.f);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = vec4(p1,1.f);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = vec4(p2,1.f);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}

//QUELLEN:
// BERECHUNG DES CENTROIDS EINES DREIECKS:
//http://mathforum.org/library/drmath/view/54899.html

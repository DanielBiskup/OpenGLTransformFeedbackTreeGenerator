#version 330

layout(triangles) in;
layout(points, max_vertices = 6) out;

in vec3[] geo_position;
in float[] geo_length; // Lenge die der zu generierende Ast haben soll.

out vec3 out_position; // Output to fragment shader
out float out_length;

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
    vec3 h = n * geo_length[0];
    vec3 c = (p0+p1+p2)/3;

    vec3 d0 = (p0 - c) * scaleTriangle;
    vec3 d1 = (p1 - c) * scaleTriangle;
    vec3 d2 = (p2 - c) * scaleTriangle;

    vec3 q0 = c + d0 + h;
    vec3 q1 = c + d1 + h;
    vec3 q2 = c + d2 + h;

    for( int i = 0; i < 3; i++) {
        out_position = geo_position[0];
        EmitVertex();
        EndPrimitive();
    }

    //RENDER STUFF!
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();

}

//QUELLEN:
// BERECHUNG DES CENTROIDS EINES DREIECKS:
//http://mathforum.org/library/drmath/view/54899.html

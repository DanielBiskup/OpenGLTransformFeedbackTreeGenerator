#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in vec3[] geo_position;
in float[] geo_length; // Lenge die der zu generierende Ast haben soll.

out vec3 out_position; // Output to fragment shader
out float out_length;

void main() {

    //UNIFORMS_BEGIN:
    float scaleTriangle = 0.8f;
    float scaleLength = 0.8f;
    //UNIFORMS_END:

    //Gegeben:
    vec3 p[3] = geo_position;

    //Gesucht:
    vec3 q[3];
    vec3 t;

    //Rechnung:
    vec3 a = p[1] - p[0];
    vec3 b = p[2] - p[0];
    vec3 n = cross(a, b);
    n = normalize(n);
    vec3 h = n * geo_length[0];
    vec3 c = (p[0]+p[1]+p[2])/3;

    //q berechnen:
    for(int i = 0; i < 3; i++) {
        vec3 d_i = (p[i] - c) * scaleTriangle;
        q[i] = c + d_i + h;
    }

    //t berechnen:
    float pyramidenHoehe = 0.3f; //TODO: Die Pyramidenhöhe irgendwie ordentlich berechnen aus den gegebenen Werten!
    t = c + h + n*pyramidenHoehe;

    for( int i = 0; i < 3; i++) {
        out_position = p[i];
        EmitVertex();
    }
    EndPrimitive();

    out_position = q[0];
    EmitVertex();
    out_position = q[1];
    EmitVertex();
    out_position = q[2];
    EmitVertex();
    EndPrimitive();
}

//QUELLEN:
// BERECHUNG DES CENTROIDS EINES DREIECKS:
//http://mathforum.org/library/drmath/view/54899.html

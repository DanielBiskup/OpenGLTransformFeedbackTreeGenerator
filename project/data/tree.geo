#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 27) out;

in vec3[] geo_position;
in float[] geo_length; // Lenge die der zu generierende Ast haben soll.

out vec3 out_position; // Output to fragment shader
out float out_length;

void emitTriangle(vec3 v0, vec3 v1, vec3 v2, float l);

void main() {

    //UNIFORMS_BEGIN:
    float scaleTriangle = 0.8f;
    float scaleLength = 0.8f;
    //UNIFORMS_END:

    //Gegeben:
    vec3 p[3] = geo_position;
    float l = geo_length[0];

    if( l <= 0.f ) {
        emitTriangle( p[0], p[1], p[2], 0.0f );
    }
    else {
        //Gesucht:
        vec3 q[3];
        vec3 t;

        //Rechnung:
        vec3 a = p[1] - p[0];
        vec3 b = p[2] - p[0];
        vec3 n = cross(a, b);
        n = normalize(n);
        vec3 h = n * l;
        vec3 c = (p[0]+p[1]+p[2])/3;

        //q berechnen:
        for(int i = 0; i < 3; i++) {
            vec3 d_i = (p[i] - c) * scaleTriangle;
            q[i] = c + d_i + h;
        }

        //t berechnen:
        float pyramidenHoehe = 0.3f; //TODO: Die Pyramidenhöhe irgendwie ordentlich berechnen aus den gegebenen Werten!
        t = c + h + n*pyramidenHoehe;

        //Erzeugen der Geometrie:
        //Schritt 1: Erzeugen der Mantelfläche:
        for( int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            emitTriangle( p[i], q[j], q[i], 0.0f );
            emitTriangle( p[i], p[j], q[j], 0.0f );
        }

        //Schritt 2: Erzeugen der Pyramide:
    }
}

void emitTriangle( vec3 v0, vec3 v1, vec3 v2, float l ) {
    out_length = l;

    out_position = v0;
    EmitVertex();

    out_position = v1;
    EmitVertex();

    out_position = v2;
    EmitVertex();

    EndPrimitive();
}

//QUELLEN:
// BERECHUNG DES CENTROIDS EINES DREIECKS:
//http://mathforum.org/library/drmath/view/54899.html

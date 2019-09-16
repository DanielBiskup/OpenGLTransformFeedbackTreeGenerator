/*
Copyright (c) 2015 Daniel Biskup
All rights reserved.

Redistribution and use in source and binary forms, with
or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

2. Redistributions in binary form must reproduce the
above copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

3. Neither the name of the copyright holder nor the names
of its contributors may be used to endorse or promote
products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 27) out;

//Uniform mit sinnvollen Standartwerten.
uniform float scaleLength = 0.7f;
uniform float scaleTriangle = 0.8f;
uniform float pyramidFactor = 0.2f;

in vec3[] geo_position;
in float[] geo_length; // Lenge die der zu generierende Ast haben soll.
in vec3[] geo_normal;

out vec3 out_position; // Output to fragment shader
out float out_length;
out vec3 out_normal;

void emitTriangle(vec3 v0, vec3 v1, vec3 v2, float l);
void emitTriangleWithNormal( vec3 v0, vec3 v1, vec3 v2, float l, vec3 normal);

void main() {
    //Gegeben:
    vec3 p[3] = geo_position;
    float l = geo_length[0];

    if( l <= 0.f ) {
        emitTriangleWithNormal( p[0], p[1], p[2], 0.0f, geo_normal[0]);
    }
    else {
        //Gesucht:
        vec3 q[3]; //Die von p extruierten Punkte
        vec3 t; //Die Spitze der Pyramide
        float l_next; //Die Länge für die Nächste Iteration

        //Rechnung:
        vec3 a = p[1] - p[0];
        vec3 b = p[2] - p[0];
        vec3 n = cross(a, b);
        n = normalize(n);
        vec3 h = n * l;
        vec3 c_p = (p[0]+p[1]+p[2])/3;

        //q berechnen:
        for(int i = 0; i < 3; i++) {
            vec3 d_i = (p[i] - c_p) * scaleTriangle;
            q[i] = c_p + d_i + h;
        }

        //float h_pyramid = length(a)*pyramidFactor;
        float h_pyramid = l * pyramidFactor;

        t = c_p + h + n * h_pyramid;

        //l_next berechnen:
        l_next = l * scaleLength;

        //Erzeugen der Geometrie:
        //Schritt 1: Erzeugen der Mantelfläche:
        for( int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            emitTriangle( p[i], q[j], q[i], 0.0f);
            emitTriangle( p[i], p[j], q[j], 0.0f);
        }

        //Schritt 2: Erzeugen der Pyramide:
        for( int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            emitTriangle( q[i], q[j], t, l_next);
        }
    }
}

void emitTriangleWithNormal( vec3 v0, vec3 v1, vec3 v2, float l, vec3 normal) {
    out_length = l;
    out_normal = normal;

    out_position = v0;
    EmitVertex();

    out_position = v1;
    EmitVertex();

    out_position = v2;
    EmitVertex();

    EndPrimitive();
}

void emitTriangle( vec3 v0, vec3 v1, vec3 v2, float l) {

    vec3 normal = normalize(cross(v1-v0, v2-v0));
    emitTriangleWithNormal(v0, v1, v2, l, normal);
}

//QUELLEN:
// BERECHUNG DES CENTROIDS EINES DREIECKS:
//http://mathforum.org/library/drmath/view/54899.html

#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//in vec3 vColor[]; // Output from vertex shader for each vertex
//out vec3 fColor; // Output to fragment shader

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}

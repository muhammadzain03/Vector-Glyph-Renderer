#version 120

void main() {
    // Pass raw 2D coordinate directly forward (Z=0, W=1)
    gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);
}

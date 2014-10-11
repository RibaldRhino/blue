#version 330
out vec4 frag_colour;
in vec3 normal_model;
void main () {
    vec3 normal = (normal_model + vec3(1.0, 1.0, 1.0))/2;
    frag_colour = vec4 (normal, 1.0);
}
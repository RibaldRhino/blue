#version 330

layout(location = 0) in vec3 vertex;

uniform mat4 modelToWorld;

layout(std140) uniform Camera
{
    mat4 worldToCamera;
    mat4 cameraToProjection;
};

void main () {
    gl_Position = cameraToProjection * worldToCamera * modelToWorld * vec4 (vertex, 1.0);
}
#include <memory>
#include <iostream>
#include <cuda_runtime_api.h>

#include "Camera.hpp"
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

Camera::Camera(World* worldPtr, Transform* transformPtr, float aspectRatio, float fieldOfView, float nearClipPlane, float farClipPlane) :
    Actor(worldPtr, transformPtr),
    _aspectRatio(aspectRatio),
    _fieldOfView(fieldOfView),
    _nearClipPlane(nearClipPlane),
    _farClipPlane(farClipPlane)
{
    _cam_speed = 4.0f; // 1 unit per second
    _cam_yaw_pitch_speed = 0.5f; // 10 degrees per second
    _cam_pos = glm::vec3(0.0f, 8.0f, 16.0f); // don't start at zero, or we will be too close
    _cam_yaw = 0.0f; // y-rotation in degrees
    _cam_pitch = 0.0f;
}

glm::mat4 Camera::getViewMatrix() {

    glm::mat4 T = glm::translate(glm::mat4(1), -_cam_pos);
    glm::mat4 Ry = glm::rotate(glm::mat4(1), -_cam_yaw, glm::vec3(0, 1, 0));
    glm::mat4 Rp = glm::rotate(glm::mat4(1), -_cam_pitch, glm::vec3(1, 0 ,0));
    glm::mat4 view_mat = Ry*Rp * T;
    return view_mat;
}

glm::mat4 Camera::getProjectionMatrix()
{
// matrix components
    float range = tan (_fieldOfView * 0.5f) * _nearClipPlane;
    float Sx = (2.0f * _nearClipPlane) / (range * _aspectRatio + range * _aspectRatio);
    float Sy = _nearClipPlane / range;
    float Sz = -(_farClipPlane + _nearClipPlane) / (_farClipPlane - _nearClipPlane);
    float Pz = -(2.0f * _farClipPlane * _nearClipPlane) / (_farClipPlane - _nearClipPlane);

    return glm::mat4(
            Sx, 0.0f, 0.0f, 0.0f,
            0.0f, Sy, 0.0f, 0.0f,
            0.0f, 0.0f, Sz, -1.0f,
            0.0f, 0.0f, Pz, 0.0f
    );
}

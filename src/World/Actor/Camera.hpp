#pragma once

#include <glm/glm.hpp>
#include "Transform/Transform.hpp"
#include "Actor.hpp"

class Camera : public Actor
{
public:
    glm::mat4 _view;
    glm::mat4 _projection;
    float _aspectRatio;
    float _fieldOfView;
    float _nearClipPlane;
    float _farClipPlane;
    float _cam_speed;
    float _cam_yaw_speed;
    glm::vec3 _cam_pos;
    float _cam_yaw;
public:
    Camera(World* worldPtr, Transform* transform = new Transform(), float aspectRatio = 4.0f/3.0f, float fieldOfView = 45.0f, float nearClipPlane = 0.1f, float farClipPlane = 100.0f);
    ~Camera() {};

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
};
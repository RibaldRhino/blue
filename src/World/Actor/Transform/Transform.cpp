#include <memory>

#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

Transform::Transform() : _model(glm::mat4(1.0f)){}

glm::mat4& Transform::getMatrix()
{
    return _model;
}

glm::vec3 Transform::getPosition()
{
    return glm::vec3(_model[3]);
}

glm::vec3 Transform::getEulerAngles()
{
    // angle = acos(( m00 + m11 + m22 - 1)/2)
    float x = (_model[2][1] - _model[1][2]) / sqrtf(powf(_model[2][1] - _model[1][2], 2.f) + powf(_model[0][2] - _model[2][0], 2.f) + powf(_model[1][0] - _model[0][1], 2.f));
    float y = (_model[0][2] - _model[2][0]) / sqrtf(powf(_model[2][1] - _model[1][2], 2.f) + powf(_model[0][2] - _model[2][0], 2.f) + powf(_model[1][0] - _model[0][1], 2.f));
    float z = (_model[1][0] - _model[0][1]) / sqrtf(powf(_model[2][1] - _model[1][2], 2.f) + powf(_model[0][2] - _model[2][0], 2.f) + powf(_model[1][0] - _model[0][1], 2.f));
    return glm::vec3(x,y,z);
}

glm::vec3 Transform::getScale()
{
    return glm::vec3(glm::length(_model[0]),glm::length(_model[1]),glm::length(_model[2]));
}

glm::vec3 Transform::getRight()
{
    return glm::vec3(_model[0][0],_model[1][0],_model[2][0]);
}

glm::vec3 Transform::getUp()
{
    return glm::vec3(_model[0][1],_model[1][1],_model[2][1]);
}

glm::vec3 Transform::getForward()
{
    return glm::vec3(_model[0][2],_model[1][2],_model[2][2]);
}

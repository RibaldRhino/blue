#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Transform {

private:
    glm::mat4 _model;

public:
    Transform();
    ~Transform() {};

    glm::mat4& getMatrix();
    glm::vec3 getPosition();
    glm::vec3 getEulerAngles();
    glm::vec3 getScale();
    glm::vec3 getForward();
    glm::vec3 getRight();
    glm::vec3 getUp();

    void Translate(glm::vec3 translation) { _model = glm::translate(_model, translation); }
    void Rotate(float angle, glm::vec3 axes) { _model = glm::rotate(_model, angle, axes); }
    void Scale(glm::vec3 scale) { _model = glm::scale(scale); }
};
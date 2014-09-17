#include <memory>

#include "Camera.hpp"

Camera::Camera(World* worldPtr, Transform* transformPtr, float aspectRatio, float fieldOfView, float nearClipPlane, float farClipPlane) :
    Actor(worldPtr, transformPtr),
    _aspectRatio(aspectRatio),
    _fieldOfView(fieldOfView),
    _nearClipPlane(nearClipPlane),
    _farClipPlane(farClipPlane)
{

}

glm::mat4* Camera::getViewMatrix()
{
    _view =  glm::lookAt(
            _transformUPtr->getPosition(),
            _transformUPtr->getPosition() + _transformUPtr->getForward(),
            _transformUPtr->getUp()
    );
    return &_view;
}

glm::mat4* Camera::getProjectionMatrix()
{
    _projection = glm::perspective(_fieldOfView, _aspectRatio, _nearClipPlane, _farClipPlane);
    return &_projection;
}

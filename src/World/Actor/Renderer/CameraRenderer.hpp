#pragma once

#include "../Camera.hpp"

class CameraRenderer
{
    private:
    Camera* _camera;
    public:
    CameraRenderer(Camera* actor);
    void Render(GLuint shader_program);
};
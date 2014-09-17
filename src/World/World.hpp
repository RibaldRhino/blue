#pragma once

#include <memory>
#include "../Process/ProcessManager.hpp"
#include "Actor/Actor.hpp"
#include "Actor/Camera.hpp"

class World
{

private:
    std::unique_ptr<ProcessManager> _processManagerUPtr;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Actor> _car;
    GLuint _shader_program;
public:
    World();
    void Render();
    void Update(double deltaTime);
};
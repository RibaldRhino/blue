#pragma once

#include <memory>
#include "../Process/ProcessManager.hpp"
#include "Actor/Actor.hpp"

class World
{

private:
    std::unique_ptr<ProcessManager> _processManagerUPtr;
    std::unique_ptr<Actor> _car;
public:
    World();
    void Render();
    void Update(double deltaTime);
};
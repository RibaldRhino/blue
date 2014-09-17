#pragma once

#include <memory>

#include "Graphics/Mesh.hpp"
#include "Transform/Transform.hpp"

class World;

class Actor
{
public:
    Actor(World* worldPtr, Transform* transformPtr = new Transform());
protected:
    World* _worldPtr;
    //std::unique_ptr<Model> _modelUPtr;
    std::unique_ptr<Transform> _transformUPtr;
};
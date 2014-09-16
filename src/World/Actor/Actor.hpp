#pragma once

#include <memory>

#include "Graphics/Mesh.hpp"
#include "Transform/Transform.hpp"
#include "Physics/Physics.hpp"
#include "../Scene/SceneNode.hpp"

class World;

class Actor
{
public:
    Actor(World* worldPtr);
protected:
    World* _worldPtr;
    //std::unique_ptr<Model> _modelUPtr;
    //std::unique_ptr<Transform> _transformUPtr;
};
#pragma once

#include <memory>

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
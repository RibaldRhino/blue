#include "Actor.hpp"

Actor::Actor(World* worldPtr, Transform* transformPtr) :
    _worldPtr(worldPtr),
    _transformUPtr(std::unique_ptr<Transform>(transformPtr)),
    _meshUPtr(std::unique_ptr<Mesh>(new Mesh()))
{

}

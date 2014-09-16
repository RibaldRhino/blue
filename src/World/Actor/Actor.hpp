#pragma once

#include <memory>

class World;

class Actor
{
protected:
    std::weak_ptr<World> _worldWPtr;
    //std::unique_ptr<Model> _modelUPtr;
    //std::unique_ptr<Transform> _transformUPtr;
    //std::unique_ptr<Physics> _physicsUPtr;
    //std::shared_ptr<SceneNode> _sceneNodeSPtr;
};
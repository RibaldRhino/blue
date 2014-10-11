#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include <Game/World/Actor/Actor.hpp>
#include "IComponent.hpp"

namespace game {

    class ModelComponent : public IComponent {
    public:
        GLuint vertexArrayObject;
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> textureCoords;
        GLuint vertexBufferObject;
        GLuint normalBufferObject;
        GLuint textureCoordsBufferObject;
        ActorWPtr _actorWPtr;
    public:
        ModelComponent(ActorWPtr actorWPtr);
        void Load(const std::string& path);
        virtual ComponentType VGetComponentType() { return ComponentType::MODEL_COMPONENT; }

    };

    typedef std::shared_ptr<ModelComponent> ModelSPtr;
}
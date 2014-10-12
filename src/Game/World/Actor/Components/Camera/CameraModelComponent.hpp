#pragma once

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <Event/IEventData.hpp>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Actor.hpp>
#include <boost/signals2/connection.hpp>

namespace game {
    class CameraModelComponent : public IComponent {
    private:
        ActorWPtr _actorWPtr;
        GLuint _cameraUBO;

        GLuint _index;

    public:
        glm::mat4 worldToCamera;
        glm::mat4 cameraToPerspective;
        CameraModelComponent(ActorWPtr actorWPtr);
        void Load(GLuint program);
        void UpdatePerspective(float aspectRatio);
        void UpdateView();
        virtual ComponentType VGetComponentType() { return ComponentType::MODEL_COMPONENT; }


    };
}
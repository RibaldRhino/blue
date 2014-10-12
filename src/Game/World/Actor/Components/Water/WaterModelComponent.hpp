#pragma once
#include <Game/World/Actor/Components/IComponent.hpp>
#include <vector>
#include <GL/glew.h>

namespace game {
    class WaterModelComponent : public IComponent {
    private:

        int _resolution;


    public:
        WaterModelComponent(int resolution, float width, float height, float depth);
        GLuint vbo;
        GLuint vao;
        std::vector<float> points;

        virtual ComponentType VGetComponentType() { return ComponentType::MODEL_COMPONENT; }


    };


}
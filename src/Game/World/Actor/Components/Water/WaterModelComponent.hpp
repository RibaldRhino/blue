#pragma once
#include <Game/World/Actor/Components/IComponent.hpp>
#include <vector>
#include <GL/glew.h>

namespace game {
    class WaterModelComponent : public IComponent {
    private:

        int _resolution;
        float _width;
        float _height;
        float _depth;

    public:
        WaterModelComponent(int resolution, float width, float height, float depth);
        GLuint position_vbo;
        GLuint radius_vbo;
        GLuint vao;
        std::vector<float> points;
        std::vector<float> radius;

        virtual ComponentType VGetComponentType() { return ComponentType::MODEL_COMPONENT; }

        float width() const { return _width; }
        float height() const { return _height; }
        float depth() const { return _depth; }
        int resolution() const { return _resolution; }
        float particle_mass() const { return (_width * _height * _depth * 1000) / _resolution; }


    };


}
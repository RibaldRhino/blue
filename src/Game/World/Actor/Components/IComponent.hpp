#pragma once
#include "ComponentType.hpp"

namespace game {
    class IComponent {
    public:
        virtual ComponentType VGetComponentType() = 0;
    };
}
#pragma once

#include <memory>
#include "Game/Process/ProcessManager.hpp"
#include "Game/World/Actor/Actor.hpp"

namespace game {

    class World {

    private:
        std::unique_ptr<process::ProcessManager> _processManagerUPtr;
        std::shared_ptr<Actor> _cameraSPtr;
        std::shared_ptr<Actor> _floorSPtr;
        std::shared_ptr<Actor> _boxSPtr;
        std::shared_ptr<Actor> _backgroundSPtr;
    public:
        World();

        void Render();

        void Update(double deltaTime);



    };
}
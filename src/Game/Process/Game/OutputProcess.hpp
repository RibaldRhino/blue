#pragma once

#include <boost/signals2/connection.hpp>
#include "Game/Process/AbstractProcess.hpp"
#include "Event/IEventData.hpp"

namespace game {
    class Game;
}

namespace process {

    class OutputProcess : public AbstractProcess {
    private:
        game::Game *_game;

    public:
        OutputProcess(game::Game *game);
        ~OutputProcess();

        void VUpdate(double deltaTime) override;

        void VInit() override;

        void VEnd() override;

    private:
        void OnWindowClose(event::IEventDataSPtr& eventData);
        boost::signals2::connection _onWindowClose;
    };
}
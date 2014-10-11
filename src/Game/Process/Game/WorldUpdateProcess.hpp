#pragma once

#include <Event/IEventData.hpp>
#include <boost/signals2/connection.hpp>
#include "Game/Process/AbstractProcess.hpp"

namespace game
{
    class Game;
}
namespace process {

    class WorldUpdateProcess : public AbstractProcess {
    private:
        game::Game *_game;

    public:
        WorldUpdateProcess(game::Game* game);
        ~WorldUpdateProcess();
        void VUpdate(double deltaTime) override;

        void VInit() override;

        void VEnd() override;


    private:
        void OnWindowClose(event::IEventDataSPtr& eventData);
        boost::signals2::connection _onWindowClose;
    };
}
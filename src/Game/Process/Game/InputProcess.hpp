#pragma once

#include <boost/signals2/connection.hpp>
#include "Game/Process/AbstractProcess.hpp"
#include "OutputProcess.hpp"


namespace game {
    class Game;
}
namespace process {
    class InputProcess : public AbstractProcess {
    private:
        game::Game* _game;

    public:

        InputProcess(game::Game *game);
        ~InputProcess();

        void VUpdate(double deltaTime) override;

        void VInit() override;

        void VEnd() override;

    private:
        void OnWindowClose(event::IEventDataSPtr& eventData);
        boost::signals2::connection _onWindowClose;
    };
}
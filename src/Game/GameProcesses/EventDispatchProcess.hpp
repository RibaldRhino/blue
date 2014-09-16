#pragma once

#include "../AbstractProcess.hpp"

class Game;

class EventDispatchProcess : public AbstractProcess
{
public:
    EventDispatchProcess(Game* game);
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;

private:
    Game *_game;
};
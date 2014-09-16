#pragma once

#include "../AbstractProcess.hpp"
#include "../Game.hpp"



class WorldUpdateProcess : public AbstractProcess
{
public:
    WorldUpdateProcess(Game* game);
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;

private:
    Game *_game;
};
#pragma once

#include "../AbstractProcess.hpp"


class Game;

class OutputProcess : public AbstractProcess
{
public:
    OutputProcess(Game* game);
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;

private:
    Game *_game;
};
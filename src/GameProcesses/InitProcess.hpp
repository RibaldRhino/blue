#pragma once

#include "../AbstractProcess.hpp"

class Game;

class InitProcess : public AbstractProcess
{
public:
    InitProcess(Game* game);
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;
};
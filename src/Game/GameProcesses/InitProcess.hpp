#pragma once

#include "../../Process/AbstractProcess.hpp"

class Game;

class InitProcess : public AbstractProcess
{
public:
    InitProcess();
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;
};
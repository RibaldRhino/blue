#pragma once

#include "../../Process/AbstractProcess.hpp"

class Game;

class InputProcess : public AbstractProcess
{
public:
    InputProcess();
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;
};
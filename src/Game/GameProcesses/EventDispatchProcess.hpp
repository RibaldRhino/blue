#pragma once

#include "../../Process/AbstractProcess.hpp"

class Game;

class EventDispatchProcess : public AbstractProcess
{
public:
    EventDispatchProcess();
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;
};
#pragma once

#include "../../../Process/AbstractProcess.hpp"
#include "../Actor.hpp"

class CarProcess : public AbstractProcess
{
public:
    CarProcess(Actor* actor);
    void VInit() override;
    void VUpdate(double deltaTime) override;
    void VEnd() override;
};
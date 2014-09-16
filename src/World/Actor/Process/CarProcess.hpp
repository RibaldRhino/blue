#pragma once

#include "../../../Process/AbstractProcess.hpp"
#include "../Actor.hpp"

class CarProcess : public AbstractProcess
{
private:
    bool _accelerationOn;
    bool _breakOn;
    bool _steerRight;
    bool _steerLeft;
public:
    CarProcess(Actor* actor);
    void VInit() override;
    void VUpdate(double deltaTime) override;
    void VEnd() override;

    Actor *_actor;
};
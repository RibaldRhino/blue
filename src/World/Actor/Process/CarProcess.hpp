#pragma once

#include "../../../Process/AbstractProcess.hpp"
#include "../Actor.hpp"
#include <glm/mat2x4.hpp>
class CarProcess : public AbstractProcess
{
private:
    bool _accelerationOn;
    bool _breakOn;
    bool _steerRight;
    bool _steerLeft;
    glm::mat2x4 mat;
public:
    CarProcess(Actor* actor);
    void VInit() override;
    void VUpdate(double deltaTime) override;
    void VEnd() override;

    Actor *_actor;
};
#pragma once

#include "../Game.hpp"



class WorldUpdateProcess : public AbstractProcess
{
public:
    WorldUpdateProcess();
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;

};
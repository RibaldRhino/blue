#pragma once

#include "../../Process/AbstractProcess.hpp"


class Game;

class OutputProcess : public AbstractProcess
{
public:
    OutputProcess();
    void VUpdate(double deltaTime) override;
    void VInit() override;
    void VEnd() override;

};
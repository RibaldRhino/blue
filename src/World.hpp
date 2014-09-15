#include <memory>
#include "ProcessManager.hpp"
#include "Actors/Water.hpp"
#include "Actors/WaterContainer.hpp"

class World
{

private:
    std::unique_ptr<Water> _waterUPtr;
    std::unique_ptr<WaterContainer> _waterContainerUPtr;
    std::unique_ptr<ProcessManager> _processManager;

public:
    World();

};
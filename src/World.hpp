#include <memory>
#include "ProcessManager.hpp"
#include "Actors/Water.hpp"
#include "Actors/WaterContainer.hpp"
#include "Actors/Camera.hpp"

class World
{

private:
    std::unique_ptr<Camera> _cameraUPtr;
    std::unique_ptr<Water> _waterUPtr;
    std::unique_ptr<WaterContainer> _waterContainerUPtr;
    std::unique_ptr<ProcessManager> _processManagerUPtr;
public:
    World();
    void Render();
    void Update(double deltaTime);

};
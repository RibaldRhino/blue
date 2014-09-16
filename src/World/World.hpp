#include <memory>
#include "../Process/ProcessManager.hpp"

class World
{

private:
    std::unique_ptr<ProcessManager> _processManagerUPtr;
public:
    World();
    void Render();
    void Update(double deltaTime);
};
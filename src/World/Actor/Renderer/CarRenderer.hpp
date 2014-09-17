#include "../Actor.hpp"

class CarRenderer
{
private:
    Actor* _actor;
public:
    CarRenderer(Actor* actor);
    void Render(GLuint shader_program);
};
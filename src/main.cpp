#include "log.hpp"
_INITIALIZE_EASYLOGGINGPP

#include <System/OpenGLSystem.hpp>
#include <System/OpenCLSystem.hpp>
#include "Game/Game.hpp"



#define GLM_FORCE_RADIANS
int main(int argc, char** argv)
{

    auto& glSystem = gamesystem::OpenGLSystem::getInstance();
    auto& clSystem = gamesystem::OpenCLSystem::getInstance();
    game::Game game;
    game.Run();
}
#include "log.hpp"
_INITIALIZE_EASYLOGGINGPP

#include <System/OpenGLSystem.hpp>
#include <System/OpenCLSystem.hpp>
#include <System/ConfigSystem.hpp>
#include "Game/Game.hpp"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#define GLM_FORCE_RADIANS
int main(int argc, char** argv)
{
    auto& glSystem = gamesystem::OpenGLSystem::getInstance();
    auto& clSystem = gamesystem::OpenCLSystem::getInstance();
    auto& configSystem = gamesystem::ConfigSystem::getInstance();

    for(int i = 1; i < argc; ++i) {
        std::vector<std::string> splitVec;
        std::string s(argv[i]);
        boost::split(splitVec, s, boost::is_any_of("="));
        if(splitVec.size() == 2)
            configSystem.put(splitVec[0],splitVec[1]);
    }

    game::Game game;
    game.Run();
}
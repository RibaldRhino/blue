#include "Game/Game.hpp"
#include "log.hpp"

_INITIALIZE_EASYLOGGINGPP

#define GLM_FORCE_RADIANS
int main(int argc, char** argv)
{
    game::Game game;
    game.Run();
}
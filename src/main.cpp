#include "Game/Game.hpp"
#include "log.hpp"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
    game::Game game;
    game.Run();
}
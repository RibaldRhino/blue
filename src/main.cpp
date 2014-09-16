#include "Game/Game.hpp"

int main(int argc, char** argv)
{
    Game& game = Game::getInstance();
    game.Run();
}
#include "OpenGLSystem.hpp"

namespace gamesystem {
    OpenGLSystem::OpenGLSystem() {
        _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", false));
    }

}

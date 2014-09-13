#pragma once

#include <memory>
#include "window.hpp"

class Game
{
    std::unique_ptr<Window> window_;
    std::shared_ptr<EventManager> outside_event_manager_;
    std::unique_ptr<ProcessManager> process_manager_;
    std::
};
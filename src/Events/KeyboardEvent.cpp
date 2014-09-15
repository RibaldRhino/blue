#include "KeyboardEvent.hpp"
#include "../EventType.hpp"

KeyboardEvent::KeyboardEvent(int key, int code, int action, int mods) :
        key(key), code(code), action(action), mods(mods)
{

}

const EventType KeyboardEvent::VGetEventType() const {
    return EventType::KEYBOARD_EVENT;
}

const double KeyboardEvent::VGetTimeStamp() const {
    return 0;
}

const char *KeyboardEvent::VGetName() const {
    return nullptr;
}

KeyboardEvent::~KeyboardEvent() {

}

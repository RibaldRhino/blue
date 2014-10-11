#pragma once

#include <Event/IEventData.hpp>
#include <glm/glm.hpp>


namespace event {
    class OnCursorPositionChanged : public IEventData {
    private:
        glm::vec2 moveVec;
    public:
        OnCursorPositionChanged(glm::vec2 vec);

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;

        const glm::vec2 getVec() const { return moveVec; }

    };
}
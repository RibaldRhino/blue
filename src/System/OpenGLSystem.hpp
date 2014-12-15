#pragma once

#include <System/Window/Window.hpp>

#include <memory>

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glx.h>

namespace gamesystem
{

    class OpenGLSystem
    {
    private:
        std::unique_ptr<Window> _windowUPtr;
    private:
        OpenGLSystem();
        OpenGLSystem(const OpenGLSystem&) = delete;
        void operator=(const OpenGLSystem&) = delete;

    public:
        static OpenGLSystem& getInstance() {
            static OpenGLSystem instance;
            return instance;
        }
        Window* getWindow() const {
            return _windowUPtr.get();
        };
        const void* getCurrentDisplay() {
            #ifdef WIN32
            return wglGetCurrentDC();
            #else
            return glXGetCurrentDisplay();
            #endif
        }
        const void* getCurrentContext() {
            #ifdef WIN32
            return wglGetCurrentContext();
            #else
            return glXGetCurrentContext();
            #endif
        }
    };
}
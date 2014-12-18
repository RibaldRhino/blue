#pragma once

#include <System/Window/Window.hpp>

#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
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
            #ifdef _WIN32
            return wglGetCurrentDC();
            #else
            return glXGetCurrentDisplay();
            #endif
        }
        const void* getCurrentContext() {
            #ifdef _WIN32
            return wglGetCurrentContext();
            #else
            return glXGetCurrentContext();
            #endif
        }
    };
}
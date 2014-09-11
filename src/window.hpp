#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
    private:
        GLFWwindow*_glfwWindow;

    public:
        Window(int width, int height, std::string title);
        ~Window();
        auto ViewportDidResize(int w, int h) -> void;

    private:
    //Callbacks
        inline static void WindowResizeCallback(GLFWwindow *glfwWindow, int w, int h);
        inline static void WindowRefreshCallback(GLFWwindow *glfwWindow);
};


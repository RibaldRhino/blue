#include <string>
#include "window.hpp"

Window::Window(int width, int height, std::string title)
{
    glfwInit();
    _glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(_glfwWindow, this);
    glfwSetFramebufferSizeCallback(_glfwWindow, WindowResizeCallback);
    glfwSetWindowRefreshCallback(_glfwWindow, WindowRefreshCallback);
}


auto Window::ViewportDidResize(int w, int h) -> void
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void inline Window::WindowRefreshCallback(GLFWwindow *glfwWindow) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
}

void inline Window::WindowResizeCallback(GLFWwindow *glfwWindow, int w, int h) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    window->ViewportDidResize(w, h);
}

Window::~Window()
{
    glfwTerminate();
}

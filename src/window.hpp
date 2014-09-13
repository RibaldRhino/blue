#pragma once

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/utility.hpp>

class Window : boost::noncopyable
{
private:
    class InputManager;
    GLFWwindow* glfw_window_;
    std::unique_ptr<InputManager> input_manager_;

public:
    Window(int width, int height, std::string title, bool fullscreen = false);
    ~Window();

private:
    class InputManager
    {
    public:
        static void OnWindowClosed(GLFWwindow *glfw_window);
        /*
[in]	focused	GL_TRUE if the window was focused, or GL_FALSE if it was defocused.
         */
        static void OnWindowFocused(GLFWwindow *glfw_window, int focused) {}
        /*
[in]	width	The new width, in pixels, of the framebuffer.
[in]	height	The new height, in pixels, of the framebuffer.
         */
        static void OnWindowFramebufferResized(GLFWwindow *glfw_window, int width, int height);
        /*
[in]	iconified	GL_TRUE if the window was iconified, or GL_FALSE if it was restored.
         */
        static void OnWindowIconify(GLFWwindow *glfw_window, int iconified) {}
        /*
[in]	xpos	The new x-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
[in]	ypos	The new y-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
         */
        static void OnWindowPositionChanged(GLFWwindow *glfw_window, int x_pos, int y_pox) {}
        static void OnWindowRefreshed(GLFWwindow *glfw_window) {}
        /*
[in]	width	The new width, in screen coordinates, of the window.
[in]	height	The new height, in screen coordinates, of the window.
        */
        static void OnWindowResized(GLFWwindow *glfw_window, int width, int height);
        /*
[in]	key	The keyboard key that was pressed or released.
[in]	scancode	The system-specific scancode of the key.
[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
[in]	mods	Bit field describing which modifier keys were held down.
         */
        static void OnKeyPressed(GLFWwindow *glfw_window, int key, int scancode, int action, int mods);
        /*
[in]	button	The mouse button that was pressed or released.
[in]	action	One of GLFW_PRESS or GLFW_RELEASE.
[in]	mods	Bit field describing which modifier keys were held down.
         */
        static void OnMouseButton(GLFWwindow *glfw_window, int button, int action, int mods);
        /*
[in]	xoffset	The scroll offset along the x-axis.
[in]	yoffset	The scroll offset along the y-axis.
         */
        static void OnScroll(GLFWwindow *glfw_window, double x_offset, double y_offset) {}
        /*
[in]	entered	GL_TRUE if the cursor entered the window's client area, or GL_FALSE if it left it.
         */
        static void OnCursorEnter(GLFWwindow *glfw_window, int entered) {}
        /*
[in]	xpos	The new x-coordinate, in screen coordinates, of the cursor.
[in]	ypos	The new y-coordinate, in screen coordinates, of the cursor.
        */
        static void OnCursorPositionChanged(GLFWwindow *glfw_window, double x_pos, double y_pos);

    };
    static void OnError(int errorcode, const char* description);
};


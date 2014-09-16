#pragma once

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/utility.hpp>

class Game;

class Window : boost::noncopyable
{
private:
    class InputManager;
    GLFWwindow*_glfwWindowPtr;
    Game* _gamePtr;
    std::unique_ptr<InputManager> _inputManagerUPtr;

public:
    Window(int width, int height, std::string title, Game *game, bool fullScreen = false);
    ~Window();

    void SwapBuffers();
    void PollEvents();

    Game* getGame() { return _gamePtr; };
    InputManager* getInputManager() { return _inputManagerUPtr.get(); }
private:
    class InputManager
    {
    public:
        double xMousePos, yMousePos;
        int mouseButton, mouseAction, mouseMods;

        static void OnWindowClosed(GLFWwindow *glfwWindow);
        /*
[in]	focused	GL_TRUE if the window was focused, or GL_FALSE if it was defocused.
         */
        static void OnWindowFocused(GLFWwindow *glfwWindow, int focused) {}
        /*
[in]	width	The new width, in pixels, of the framebuffer.
[in]	height	The new height, in pixels, of the framebuffer.
         */
        static void OnWindowFramebufferResized(GLFWwindow *glfwWindow, int width, int height);
        /*
[in]	iconified	GL_TRUE if the window was iconified, or GL_FALSE if it was restored.
         */
        static void OnWindowIconify(GLFWwindow *glfwWindow, int iconified) {}
        /*
[in]	xpos	The new x-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
[in]	ypos	The new y-coordinate, in screen coordinates, of the upper-left corner of the client area of the window.
         */
        static void OnWindowPositionChanged(GLFWwindow *glfwWindow, int x_pos, int y_pox) {}
        static void OnWindowRefreshed(GLFWwindow *glfwWindow) {}
        /*
[in]	width	The new width, in screen coordinates, of the window.
[in]	height	The new height, in screen coordinates, of the window.
        */
        static void OnWindowResized(GLFWwindow *glfwWindow, int width, int height);
        /*
[in]	key	The keyboard key that was pressed or released.
[in]	scancode	The system-specific scancode of the key.
[in]	action	GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
[in]	mods	Bit field describing which modifier keys were held down.
         */
        static void OnKeyPressed(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);
        /*
[in]	button	The mouse button that was pressed or released.
[in]	action	One of GLFW_PRESS or GLFW_RELEASE.
[in]	mods	Bit field describing which modifier keys were held down.
         */
        static void OnMouseButton(GLFWwindow *glfwWindow, int button, int action, int mods);
        /*
[in]	xoffset	The scroll offset along the x-axis.
[in]	yoffset	The scroll offset along the y-axis.
         */
        static void OnScroll(GLFWwindow *glfwWindow, double x_offset, double y_offset) {}
        /*
[in]	entered	GL_TRUE if the cursor entered the window's client area, or GL_FALSE if it left it.
         */
        static void OnCursorEnter(GLFWwindow *glfwWindow, int entered) {}
        /*
[in]	xpos	The new x-coordinate, in screen coordinates, of the cursor.
[in]	ypos	The new y-coordinate, in screen coordinates, of the cursor.
        */
        static void OnCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos);
    };
    static void OnError(int errorCode, const char* description);
};


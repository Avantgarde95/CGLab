#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include "Engine.hpp"

namespace Engine {
    // Class for using GLFW and GLEW easily. Just make a subclass, override onXXX(), and call run().
    class Window {
    public:
        Window(int width, int height, const std::string& title);

        // Create the window and start rendering. When it stops, close the window and return the exit code.
        int run();

    protected:
        // Called when we're ready to render.
        virtual void onStart() {};
        // Called in each frame.
        virtual void onDraw() {};
        // Called when rendering is terminated.
        virtual void onEnd() {};

        // Called when the size of the window is changed.
        virtual void onSizeChange(int width, int height) {};
        // Called when we click the mouse.
        virtual void onMouseClick(float x, float y) {};
        // Called when we move the mouse.
        virtual void onMouseMove(float x, float y) {};
        // Called when we press the key.
        virtual void onKeyPress(int key) {};

    private:
        static void windowSizeCallback(GLFWwindow* context, int width, int height);
        static void mouseButtonCallback(GLFWwindow* context, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* context, double x, double y);
        static void keyCallback(GLFWwindow* context, int key, int scanCode, int action, int mods);

        // Window width.
        int m_windowWidth;
        // Window height.
        int m_windowHeight;

        // Frame buffer width.
        int m_frameBufferWidth;
        // Frame buffer height.
        int m_frameBufferheight;

        // Window title.
        std::string m_title;

        // GLFW window object.
        GLFWwindow* m_context;
    };
}

#endif

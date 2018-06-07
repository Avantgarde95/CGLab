#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP

#include "Engine.hpp"

namespace Engine {
    // Class for using GLFW and GLEW easily. Just make a subclass, override onXXX(), and call run().
    class Renderer {
    public:
        Renderer(int width, int height, const std::string &title);

        // Start rendering.
        void run();

    protected:
        // Called in each frame.
        virtual void onDraw() {};

        // Called when the size of the window is changed.
        virtual void onSizeChange(int width, int height) {};

        // Called when we click the mouse.
        virtual void onMouseClick(float x, float y) {};

        // Called when we move the mouse.
        virtual void onMouseMove(float x, float y) {};

        // Called when we press the key.
        virtual void onKeyPress(int key) {};

        // Called when we release the key.
        virtual void onKeyRelease(int key) {};

    private:
        static void windowSizeCallback(GLFWwindow *context, int width, int height);
        static void mouseButtonCallback(GLFWwindow *context, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow *context, double x, double y);
        static void keyCallback(GLFWwindow *context, int key, int scanCode, int action, int mods);

        // Window size.
        int m_windowWidth;
        int m_windowHeight;

        // Frame buffer size.
        int m_frameBufferWidth;
        int m_frameBufferHeight;

        // Window title.
        std::string m_title;

        // GLFW window object.
        GLFWwindow *m_window;
    };
}

#endif


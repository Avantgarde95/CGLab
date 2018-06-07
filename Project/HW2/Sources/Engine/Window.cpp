#include "Engine.hpp"

namespace Engine {
    // Map for finding Window from GLFWwindow.
    static std::map<GLFWwindow*, Window*> windowMap;

    Window::Window(int width, int height, const std::string& title)
        : m_windowWidth(width), m_windowHeight(height), m_title(title) {
    }

    int Window::run() {
        // Initialize GLFW.
        if (!glfwInit()) {
            std::cout << "Error: Failed to initialize GLFW.\n";
            std::cin.get();
            return 1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create the window.
        m_context = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title.c_str(), nullptr, nullptr);
        windowMap[m_context] = this;

        if (m_context == nullptr) {
            glfwTerminate();
            std::cout << "Error: Failed to create the window.\n";
            std::cin.get();
            return 1;
        }

        glfwMakeContextCurrent(m_context);

        // Initialize GLEW.
        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            std::cout << "Error: Failed to initialize GLEW.\n";
            std::cin.get();
            return 1;
        }

        // Set the callbacks.
        glfwSetInputMode(m_context, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetWindowSizeCallback(m_context, Window::windowSizeCallback);
        glfwSetMouseButtonCallback(m_context, Window::mouseButtonCallback);
        glfwSetCursorPosCallback(m_context, Window::cursorPosCallback);
        glfwSetKeyCallback(m_context, Window::keyCallback);

        // Set the size of the frame buffer.
        glfwGetFramebufferSize(
            m_context,
            &m_frameBufferWidth,
            &m_frameBufferheight
        );

        onSizeChange(m_frameBufferWidth, m_frameBufferheight);

        // Render until ESCAPE key or X button is pressed.
        onStart();

        do {
            onDraw();
            glfwSwapBuffers(m_context);
            glfwPollEvents();
        } while (glfwGetKey(m_context, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && !glfwWindowShouldClose(m_context));

        onEnd();

        // Close the window.
        glfwTerminate();

        return 0;
    }

    void Window::windowSizeCallback(GLFWwindow* context, int width, int height) {
        auto window = windowMap[context];

        window->m_windowWidth = width;
        window->m_windowHeight = height;

        glfwGetFramebufferSize(
            window->m_context,
            &(window->m_frameBufferWidth),
            &(window->m_frameBufferheight)
        );

        window->onSizeChange(window->m_frameBufferWidth, window->m_frameBufferheight);
    }

    void Window::mouseButtonCallback(GLFWwindow* context, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            auto window = windowMap[context];
            double x, y;

            glfwGetCursorPos(window->m_context, &x, &y);

            window->onMouseClick(
                static_cast<float>(x / (window->m_windowWidth * window->m_frameBufferWidth)),
                static_cast<float>(y / (window->m_windowWidth * window->m_frameBufferheight))
            );
        }
    }

    void Window::cursorPosCallback(GLFWwindow* context, double x, double y) {
        auto window = windowMap[context];
        window->onMouseMove(static_cast<float>(x), static_cast<float>(y));
    }

    void Window::keyCallback(GLFWwindow* context, int key, int scanCode, int action, int mods) {
        if (action == GLFW_PRESS) {
            auto window = windowMap[context];
            windowMap[context]->onKeyPress(key);
        }
    }
}

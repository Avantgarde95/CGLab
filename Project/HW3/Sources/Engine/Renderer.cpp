#include "Engine.hpp"

// Map for finding Renderer from GLFWwindow.
static std::map<GLFWwindow *, Engine::Renderer *> windowMap;

namespace Engine {
    Renderer::Renderer(int width, int height, const std::string &title)
            : m_windowWidth(width), m_windowHeight(height), m_title(title) {
        // Initialize GLFW.
        if (!glfwInit()) {
            throw std::runtime_error("Error: Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create the window.
        m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_title.c_str(), nullptr, nullptr);
        windowMap[m_window] = this;

        if (m_window == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Error: Failed to create the window.");
        }

        glfwMakeContextCurrent(m_window);

        // Initialize GLEW.
        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Error: Failed to initialize GLEW.");
        }

        // Set the callbacks.
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetWindowSizeCallback(m_window, Renderer::windowSizeCallback);
        glfwSetMouseButtonCallback(m_window, Renderer::mouseButtonCallback);
        glfwSetCursorPosCallback(m_window, Renderer::cursorPosCallback);
        glfwSetKeyCallback(m_window, Renderer::keyCallback);

        // Set the size of the frame buffer.
        glfwGetFramebufferSize(
                m_window,
                &m_frameBufferWidth,
                &m_frameBufferHeight
        );
    }

    void Renderer::run() {
        onSizeChange(m_frameBufferWidth, m_frameBufferHeight);

        // Render until ESCAPE key or X button is pressed.
        do {
            onDraw();
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        } while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS
                 && !glfwWindowShouldClose(m_window));

        // Close the window.
        glfwTerminate();
    }

    void Renderer::windowSizeCallback(GLFWwindow *context, int width, int height) {
        auto window = windowMap[context];

        window->m_windowWidth = width;
        window->m_windowHeight = height;

        glfwGetFramebufferSize(
                window->m_window,
                &(window->m_frameBufferWidth),
                &(window->m_frameBufferHeight)
        );

        window->onSizeChange(window->m_frameBufferWidth, window->m_frameBufferHeight);
    }

    void Renderer::mouseButtonCallback(GLFWwindow *context, int button, int action, int /* mods */) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            auto window = windowMap[context];
            double x, y;

            glfwGetCursorPos(window->m_window, &x, &y);

            window->onMouseClick(
                    static_cast<float>(x / (window->m_windowWidth * window->m_frameBufferWidth)),
                    static_cast<float>(y / (window->m_windowWidth * window->m_frameBufferHeight))
            );
        }
    }

    void Renderer::cursorPosCallback(GLFWwindow *context, double x, double y) {
        auto window = windowMap[context];
        window->onMouseMove(
                static_cast<float>(x * 2.0f / window->m_frameBufferWidth - 1.0f),
                static_cast<float>(y * 2.0f / window->m_frameBufferHeight - 1.0f)
        );
    }

    void Renderer::keyCallback(GLFWwindow *context, int key, int /* scanCode */, int action, int /* mods */) {
        auto window = windowMap[context];

        switch (action) {
        case GLFW_PRESS:
            window->onKeyPress(key);
            break;
        case GLFW_RELEASE:
            window->onKeyRelease(key);
            break;
        default:
            break;
        }
    }
}

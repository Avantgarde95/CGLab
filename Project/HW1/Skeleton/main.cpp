// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include common libraries
#include "../common/geometry.hpp"
#include "../common/model.hpp"

using namespace std;
using namespace glm;

// Global variable
GLFWwindow* g_window;
float g_window_width = 800;
float g_window_height = 600;
int g_framebuffer_width = static_cast<int>(g_window_width);
int g_framebuffer_height = static_cast<int>(g_window_height);
float g_fov = 45.f; // 45-degree of fov
float g_fovy = g_fov; // y-angle of fov

// I don't need these variables.
// std::vector<Model> g_models;
// std::vector<mat4> g_model_rbts;

// Timer
Timer timer;

// Mobile
Mobile mobile(10);

// Transformations
mat4 g_projection;
// Camera model property
mat4 g_eye_rbt;
// World model property
mat4 g_world_rbt = mat4(1.0f);

// Event functions
static void WindowSizeCallback(GLFWwindow*, int, int);
static void MouseButtonCallback(GLFWwindow*, int, int, int);
static void CursorPosCallback(GLFWwindow*, double, double);
static void KeyboardCallback(GLFWwindow*, int, int, int, int);
void UpdateFovy(void);
void CleanUp(void);

// Helper function: Update the vertical field-of-view(float fovy in global)
void UpdateFovy() {
    if (g_framebuffer_width >= g_framebuffer_height) {
        g_fovy = g_fov;
    }
    else {
        const float RAD_PER_DEG = 0.5f * glm::pi<float>() / 180.0f;
        g_fovy = (float)atan2(
            sin(g_fov * RAD_PER_DEG) * ((float)g_framebuffer_height / (float)g_framebuffer_width),
            cos(g_fov * RAD_PER_DEG)
        ) / RAD_PER_DEG;
    }
}

void CleanUp() {
}

// TODO: Modify GLFW window resized callback function
static void WindowSizeCallback(GLFWwindow* a_window, int a_width, int a_height) {
    // Get resized size and set to current window
    g_window_width = (float)a_width;
    g_window_height = (float)a_height;

    // glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
    // window size != framebuffer size
    glfwGetFramebufferSize(a_window, &g_framebuffer_width, &g_framebuffer_height);
    glViewport(0, 0, (GLsizei)g_framebuffer_width, (GLsizei)g_framebuffer_height);

    // Update projection matrix
    g_projection = perspective(
        g_fov,
        ((float)g_framebuffer_width / (float)g_framebuffer_height),
        0.1f, 100.0f
    );
}

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods) {
    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
        ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
    }
}

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos) {

}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods) {
    if (a_action == GLFW_PRESS) {
        switch (a_key) {
        case GLFW_KEY_H:
            cout
                << "CS580 Homework Assignment 1\n"
                << "keymaps:\n"
                << "H(h)\t\t Help command\n"
                // << "p\t\t Enable/Disable picking\n"
                << "O(o)\t\t Select the next node\n"
                << "M(m)\t\t Increase the speed of the current node\n"
                << "N(n)\t\t Decrease the speed of the current node\n"
                << "R(r)\t\t Reset the speed of the current node\n"
                ;
            break;
        case GLFW_KEY_O:
            mobile.selectNextNode();
            break;
        case GLFW_KEY_M:
            printf("Increasing the speed of the current node\n");

            if (true) {
                MobileNode& node = mobile.getCurrentNode();

                node.setRotationSpeed(
                    node.getRotationSpeed()
                    + min(abs(10.0f / (timer.getFrameCount() + 1)), 0.1f)
                );
            }

            break;
        case GLFW_KEY_N:
            printf("Decreasing the speed of the current node\n");

            if (true) {
                MobileNode& node = mobile.getCurrentNode();

                node.setRotationSpeed(
                    node.getRotationSpeed()
                    - min(abs(10.0f / (timer.getFrameCount() + 1)), 0.1f)
                );
            }

            break;
        case GLFW_KEY_R:
            printf("Resetting the speed of the current node\n");
            mobile.getCurrentNode().setRotationSpeed(0.0f);
        default:
            break;
        }
    }
}

int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a window and create its OpenGL context
    g_window = glfwCreateWindow(
        static_cast<int>(g_window_width), static_cast<int>(g_window_height),
        "Homework 1: 20130295 - Hunmin Park",
        NULL, NULL
    );

    if (g_window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowSizeCallback(g_window, WindowSizeCallback);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);

    // Clear with black backround
    glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_ALWAYS);
    // Enable culling
    glEnable(GL_CULL_FACE);
    // Backface culling
    glCullFace(GL_BACK);

    // Setting viewport
    // glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
    // window size != framebuffer size
    glfwGetFramebufferSize(g_window, &g_framebuffer_width, &g_framebuffer_height);
    glViewport(0, 0, (GLsizei)g_framebuffer_width, (GLsizei)g_framebuffer_height);

    g_projection = perspective(
        g_fov, // fovy
        ((float)g_framebuffer_width / (float)g_framebuffer_height), // aspect
        0.1f,  // near
        100.0f // far
    );

    g_eye_rbt = inverse(lookAt(
        vec3(5.0f, 0.3f, 3.0f), // eye
        vec3(0.0f, -1.3f, 0.0f),  // center
        vec3(0.0f, 1.0f, 0.0f)    // up
    ));

    // Initialize the mobile.
    vec3 g_light_direction(0, -1, 1);

    mobile[0]
        .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
        .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
        .setTranslation(vec3(0.0f, 0.0f, 0.0f))
        .addShape(
            Shape()
            .setAttribute("height", 0.4f)
            .setAttribute("radius", 0.2f)
            .setAttribute("color", vec3(0.0f, 1.0f, 0.0f))
            .buildCylinder()
        )
        .addChild(
            mobile[1]
            .setTopVertex(vec3(0.0f, 0.25f, 0.0f))
            .setBottomVertex(vec3(0.0f, -0.1f, 0.0f))
            .setTranslation(vec3(1.5f, -1.3f, 0.0f))
            .addShape(
                Shape()
                .setAttribute("height", 0.1f)
                .setAttribute("radius", 0.05f)
                .setAttribute("color", vec3(0.0f, 0.3f, 0.3f))
                .setMatrix(translate(vec3(0.0f, 0.2f, 0.0f)))
                .buildCylinder()
            ).addShape(
                Shape()
                .setAttribute("height", 0.1f)
                .setAttribute("radius", 0.1f)
                .setAttribute("color", vec3(0.0f, 0.3f, 0.3f))
                .setMatrix(translate(vec3(0.0f, 0.1f, 0.0f)))
                .buildCylinder()
            ).addShape(
                Shape()
                .setAttribute("height", 0.2f)
                .setAttribute("radius", 0.2f)
                .setAttribute("color", vec3(0.3f, 0.3f, 0.3f))
                .buildCylinder()
            ).addChild(
                mobile[2]
                .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                .setTranslation(vec3(0.5f, -0.8f, 0.0f))
                .addShape(
                    Shape()
                    .setAttribute("height", 0.4f)
                    .setAttribute("radius", 0.3f)
                    .setAttribute("level", 3)
                    .setAttribute("color", vec3(0.3f, 0.3f, 0.0f))
                    .buildPrism()
                )
            ).addChild(
                mobile[3]
                .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                .setTranslation(vec3(-0.5f, -0.8f, 0.0f))
                .addShape(
                    Shape()
                    .setAttribute("height", 0.4f)
                    .setAttribute("radius", 0.3f)
                    .setAttribute("level", 4)
                    .setAttribute("color", vec3(0.3f, 0.0f, 0.3f))
                    .buildPyramid()
                )
            )
        ).addChild(
            mobile[4]
            .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
            .setBottomVertex(vec3(0.0f, -0.1f, 0.0f))
            .setTranslation(vec3(-1.5f * 0.5f, -1.3f, 1.5f * 0.866f))
            .addShape(
                Shape()
                .setAttribute("height", 0.1f)
                .setAttribute("radius", 0.3f)
                .setAttribute("color", vec3(0.3f, 0.3f, 1.0f))
                .setMatrix(translate(vec3(0.0f, 0.15f, 0.0f)))
                .buildCone()
            ).addShape(
                Shape()
                .setAttribute("height", 0.1f)
                .setAttribute("radius", 0.3f)
                .setAttribute("color", vec3(1.0f, 0.0f, 0.0f))
                .buildCylinder()
            ).addChild(
                mobile[5]
                .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                .setTranslation(vec3(0.6f, -0.5f, 0.0f))
                .addShape(
                    Shape()
                    .setAttribute("height", 0.4f)
                    .setAttribute("radius", 0.3f)
                    .setAttribute("color", vec3(0.2f, 0.4f, 0.6f))
                    .buildCone()
                )
            ).addChild(
                mobile[6]
                .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                .setTranslation(vec3(-0.6f, -0.5f, 0.0f))
                .addShape(
                    Shape()
                    .setAttribute("height", 0.4f)
                    .setAttribute("radius", 0.3f)
                    .setAttribute("color", vec3(0.5f, 0.3f, 0.1f))
                    .buildCone()
                )
            )
        ).addChild(
            mobile[7]
            .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
            .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
            .setTranslation(vec3(-1.5f * 0.5f, -1.3f, -1.5f * 0.866f))
            .addShape(
                Shape()
                .setAttribute("height", 0.4f)
                .setAttribute("radius", 0.3f)
                .setAttribute("color", vec3(0.1f, 0.3f, 1.4f))
                .buildCone()
            ).addChild(
                mobile[8]
                .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                .setTranslation(vec3(0.2f, -0.8f, 0.0f))
                .addShape(
                    Shape()
                    .setAttribute("height", 0.4f)
                    .setAttribute("radius", 0.4f)
                    .setAttribute("color", vec3(0.4f, 0.4f, 1.4f))
                    .setAttribute("level", 4)
                    .buildPrism()
                ).addChild(
                    mobile[9]
                    .setTopVertex(vec3(0.0f, 0.2f, 0.0f))
                    .setBottomVertex(vec3(0.0f, -0.2f, 0.0f))
                    .setTranslation(vec3(0.0f, -0.8f, 0.0f))
                    .addShape(
                        Shape()
                        .setAttribute("height", 0.4f)
                        .setAttribute("radius", 0.3f)
                        .setAttribute("color", vec3(0.1f, 0.1f, 1.0f))
                        .buildCylinder()
                    )
                )
            )
        );

    mobile.create();

    do {
        // Clear the screen	
        // Fill the background
        glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f, (GLclampf)0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tick the timer.
        timer.tick();

        // Draw the mobile.
        mobile[0].draw(
            g_projection,
            g_eye_rbt,
            g_light_direction
        );

        // Swap buffers
        glfwSwapBuffers(g_window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(g_window) == 0);

    // Cleanup all resources
    CleanUp();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

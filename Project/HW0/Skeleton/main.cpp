#include <cstdio>
#include <vector>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../common/shader.hpp"

class Window {
public:
    int width;
    int height;
    std::string title;
    GLFWwindow* context;
    bool isSizeChanged;

    Window(int width, int height, std::string title) {
        this->width = width;
        this->height = height;
        this->title = title;
    }

    float getAspect() {
        return float(width) / float(height);
    }

    bool initGLFW() {
        if (!glfwInit()) {
            std::fprintf(stderr, "Failed to initialize GLFW\n");

            return false;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        return true;
    }

    void closeGLFW() {
        glfwTerminate();
    }

    bool openWindow() {
        context = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (context == NULL) {
            std::fprintf(stderr, "Failed to open GLFW window.\n");
            closeGLFW();

            return false;
        }

        glfwMakeContextCurrent(context);
        glfwSetInputMode(context, GLFW_STICKY_KEYS, GL_TRUE);

        return true;
    }

    void swapBuffers() {
        glfwSwapBuffers(context);
        glfwPollEvents();
    }

    bool updateSize() {
        int oldWidth = width;
        int oldHeight = height;

        glfwGetFramebufferSize(context, &width, &height);

        // return true if the size has been changed
        return (width == oldWidth) && (height == oldHeight);
    }

    bool isESCPressed() {
        return glfwGetKey(context, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    }

    bool isWindowClosed() {
        return glfwWindowShouldClose(context) != 0;
    }
};

class Timer {
public:
    double lastTime;
    int frameCount;

    Timer() {
        this->lastTime = 0;
        this->frameCount = 0;
    }

    void check() {
        double currTime = glfwGetTime();

        frameCount++;

        if (currTime - lastTime >= 1.0) {
            std::printf(
                "%f ms/frame (%d frames)\n",
                1000.0 / double(frameCount), frameCount
            );
            frameCount = 0;
            lastTime += 1.0;
        }
    }
};

class MyModel {
private:
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<GLuint> triangleIndices;
    std::vector<GLuint> lineIndices;

    GLuint programID;
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint triangleIndexBufferID;
    GLuint lineIndexBufferID;
    GLuint colorBufferID;
    GLuint modelMatrixID;
    GLuint viewMatrixID;
    GLuint projectionMatrixID;
    GLuint frameCountID;

public:
    MyModel() {
        // do nothing
    }

    void addVertex(float x, float y, float z) {
        vertices.push_back(glm::vec3(x, y, z));
    }

    void addColor(float r, float g, float b) {
        colors.push_back(glm::vec3(r, g, b));
    }

    void addTriangle(int index1, int index2, int index3) {
        triangleIndices.push_back(index1);
        triangleIndices.push_back(index2);
        triangleIndices.push_back(index3);

        lineIndices.push_back(index1);
        lineIndices.push_back(index2);
        lineIndices.push_back(index2);
        lineIndices.push_back(index3);
        lineIndices.push_back(index3);
        lineIndices.push_back(index1);
    }

    void setViewMatrix(
        float eyeX, float eyeY, float eyeZ,
        float centerX, float centerY, float centerZ,
        float upX, float upY, float upZ
    ) {
        viewMatrix = glm::lookAt(
            glm::vec3(eyeX, eyeY, eyeZ),
            glm::vec3(centerX, centerY, centerZ),
            glm::vec3(upX, upY, upZ)
        );
    }

    void setProjectionMatrix(float fovY, float aspect, float zNear, float zFar) {
        projMatrix = glm::perspective(fovY, aspect, zNear, zFar);
    }

    void setShader(std::string vertexShaderPath, std::string fragmentShaderPath) {
        // shaders
        programID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

        // vertex data
        glGenVertexArrays(1, &vertexArrayID);

        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(glm::vec3),
            &vertices[0],
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(
            GL_ARRAY_BUFFER,
            colors.size() * sizeof(glm::vec3),
            &colors[0],
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &triangleIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBufferID);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            triangleIndices.size() * sizeof(GLuint),
            &triangleIndices[0],
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &lineIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndexBufferID);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            lineIndices.size() * sizeof(GLuint),
            &lineIndices[0],
            GL_STATIC_DRAW
        );

        // uniform data
        modelMatrixID = glGetUniformLocation(programID, "modelMatrix");
        viewMatrixID = glGetUniformLocation(programID, "viewMatrix");
        projectionMatrixID = glGetUniformLocation(programID, "projectionMatrix");
        frameCountID = glGetUniformLocation(programID, "frameCount");
    }

    void draw(bool drawTriangles, bool drawLines, int frameCount) {
        // pass vertex data
        glUseProgram(programID);

        glBindVertexArray(vertexArrayID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized
            0,        // stride
            (void*)0  // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(
            1,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized
            0,        // stride
            (void*)0  // array buffer offset
        );

        // pass uniform data
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projMatrix[0][0]);
        glUniform1i(frameCountID, frameCount);

        // draw the polygons
        if (drawTriangles) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBufferID);
            glDrawElements(
                GL_TRIANGLES,
                triangleIndices.size(),
                GL_UNSIGNED_INT,
                (void*)0
            );
        }

        if (drawLines) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndexBufferID);
            glDrawElements(
                GL_LINES,
                lineIndices.size(),
                GL_UNSIGNED_INT,
                (void*)0
            );
        }
    }
};

int main() {
    Window window(400, 300, "OpenGL Tutorial: 20130295");
    MyModel model;
    Timer timer;

    // initialize GLFW & open the window
    if (!(window.initGLFW() && window.openWindow())) {
        return -1;
    }

    // initialize GLEW
    glewExperimental = true; // Needed for core profile

    if (glewInit() != GLEW_OK) {
        std::fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // initialize GL
    // -- background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // -- enable depth test
    glEnable(GL_DEPTH_TEST);
    // -- accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // -- enable back-face culling
    glEnable(GL_CULL_FACE);

    // set the viewport
    window.updateSize();
    glViewport(0, 0, window.width, window.height);

    // prepare the model
    // -- vertices & colors
    float coordinates[8][3] = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };

    for (int i = 0; i < 8; i++) {
        model.addVertex(coordinates[i][0], coordinates[i][1], coordinates[i][2]);

        if (coordinates[i][0] == 0 && coordinates[i][1] == 0 && coordinates[i][2] == 0) {
            // use gray instead of black (since we can't see the black vertex)
            model.addColor(0.3, 0.3, 0.3);
        }
        else {
            model.addColor(coordinates[i][0], coordinates[i][1], coordinates[i][2]);
        }
    }

    // -- indices
    // ---- front
    model.addTriangle(3, 6, 7);
    model.addTriangle(5, 3, 7);

    // ---- back
    model.addTriangle(0, 2, 1);
    model.addTriangle(2, 4, 1);

    // ---- right
    model.addTriangle(1, 4, 7);
    model.addTriangle(7, 6, 1);

    // ---- left
    model.addTriangle(5, 2, 3);
    model.addTriangle(3, 2, 0);

    // ---- top
    model.addTriangle(2, 5, 7);
    model.addTriangle(2, 7, 4);

    // ---- below
    model.addTriangle(0, 1, 3);
    model.addTriangle(1, 6, 3);

    // transform
    // -- model: Identity

    // -- camera
    model.setViewMatrix(
        3.0f, 3.5f, 4.0f, // eye
        0.0f, 0.0f, 0.0f,  // center
        0.0f, 1.0f, 0.0f   // up
    );

    // -- projection
    model.setProjectionMatrix(
        45.0f,              // fov
        window.getAspect(), // aspect
        0.1f,               // near
        100.0f              // far
    );

    // set the shader (and transfer the data to GPU)
    model.setShader(
        "TransformVertexShader.glsl",
        "ColorFragmentShader.glsl"
    );

    // initialize the timer
    timer.check();

    do {
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the viewport and the model when the window size is changed
        if (window.updateSize()) {
            glViewport(0, 0, window.width, window.height);

            model.setProjectionMatrix(
                45.0f,              // fov
                window.getAspect(), // aspect
                0.1f,               // near
                100.0f              // far
            );
        }

        timer.check();

        if (timer.frameCount <= 1000) {
            model.draw(true, true, timer.frameCount);
        }
        else {
            model.draw(false, true, timer.frameCount);
        }

        window.swapBuffers();
    } while (!window.isESCPressed() && !window.isWindowClosed());

    window.closeGLFW();
    return 0;
}

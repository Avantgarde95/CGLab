#include "App/App.hpp"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

static const GLsizei INITIAL_WIDTH = 600;
static const GLsizei INITIAL_HEIGHT = 500;
static std::string TEXTURE_PATH = "Resources/Images/"; // NOLINT
static std::string SHADER_PATH = "Resources/Shaders/"; // NOLINT
static std::string MODEL_PATH = "Resources/Models/"; // NOLINT

class MyRenderer : public Engine::Renderer {
private:
    // Textures.
    Engine::Texture skyTexture{TEXTURE_PATH + "DarkSky.png"};
    Engine::Texture jesusTexture{TEXTURE_PATH + "Jesus.png"};
    Engine::Texture catLightTexture{TEXTURE_PATH + "CatLight.png"};
    Engine::Texture catDarkTexture{TEXTURE_PATH + "CatDark.png"};
    Engine::Texture chopperTexture{TEXTURE_PATH + "Chopper.png"};
    Engine::Texture landTexture{TEXTURE_PATH + "Land.png"};
    Engine::Texture lightTexture{TEXTURE_PATH + "Yellow.png"};
    Engine::Texture brushTexture{TEXTURE_PATH + "Brush.png"};

    // Frame buffers.
    // -- For shadow mapping.
    Engine::FrameBuffer depthFrameBuffer{INITIAL_WIDTH, INITIAL_HEIGHT};
    // -- For post processing.
    Engine::FrameBuffer drawFrameBuffer{INITIAL_WIDTH, INITIAL_HEIGHT};

    // Vertex shaders.
    Engine::Shader depthVertexShader{Engine::Shader::Type::VERTEX, SHADER_PATH + "Depth.vert"};
    Engine::Shader drawVertexShader{Engine::Shader::Type::VERTEX, SHADER_PATH + "Draw.vert"};
    Engine::Shader displayVertexShader{Engine::Shader::Type::VERTEX, SHADER_PATH + "Display.vert"};

    // Fragment shaders.
    Engine::Shader depthFragmentShader{Engine::Shader::Type::FRAGMENT, SHADER_PATH + "Depth.frag"};
    Engine::Shader drawFragmentShader{Engine::Shader::Type::FRAGMENT, SHADER_PATH + "Draw.frag"};
    Engine::Shader displayFragmentShader{Engine::Shader::Type::FRAGMENT, SHADER_PATH + "Display.frag"};

    // Programs.
    Engine::Program depthProgram{&depthVertexShader, &depthFragmentShader};
    Engine::Program drawProgram{&drawVertexShader, &drawFragmentShader};
    Engine::Program displayProgram{&displayVertexShader, &displayFragmentShader};

    // Lights.
    // -- Background light. (Dark blue, directional)
    Engine::Light backgroundLight{
            Engine::Light::Type::DIRECTIONAL,
            glm::vec3(0.0f, 4.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 0.2f, 0.4f),
            glm::vec3(0.0f, 0.2f, 0.4f),
            glm::vec3(0.0f, 0.2f, 0.4f),
            0.0f,
            0.0f
    };

    // -- Main light. This light makes the shadows. (Yellow, point light)
    Engine::Light mainLight{
            Engine::Light::Type::POINT,
            glm::vec3(3.0f, 6.0f, 3.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            0.0f,
            0.03f
    };

    // Models.
    // -- Skybox.
    App::SkyModel skyModel{
            {30.0f, 30.0f, 30.0f},
            {0.0f,  0.0f,  0.0f}
    };

    // -- Land.
    App::LandModel landModel{
            {30.0f, 30.0f},
            0.0f
    };

    // -- General models.
    App::ExternalModel jesusModel{MODEL_PATH + "Jesus.obj"};
    App::ExternalModel catModel1{MODEL_PATH + "Cat.obj"};
    App::ExternalModel catModel2{MODEL_PATH + "Cat.obj"};
    App::ExternalModel chopperModel{MODEL_PATH + "Chopper.obj"};

    // -- My character.
    App::ExternalModel myModel{MODEL_PATH + "Cat.obj"};

    // -- Light model. (Yellow cat)
    App::ExternalModel lightModel{MODEL_PATH + "Cat.obj"};

    // -- Simple rectangle. We'll draw the models on this and apply post processing.
    App::DisplayModel displayModel{1.8f};

    // Model groups.
    std::vector<App::GeneralModel *> drawModelGroup{
            &myModel,
            &lightModel,
            &skyModel,
            &landModel,
            &jesusModel,
            &catModel1,
            &catModel2,
            &chopperModel
    };

    std::vector<App::GeneralModel *> shadowModelGroup{
            &myModel,
            &lightModel,
            &jesusModel,
            &catModel1,
            &catModel2,
            &chopperModel
    };

    std::vector<App::GeneralModel *> selectModelGroup{
            &jesusModel,
            &catModel1,
            &catModel2,
            &chopperModel,
            &landModel
    };

    int selectedModelIndex = 0;

    // Matrices.
    // -- Light's projection matrix & view matrix for shadow mapping.
    glm::mat4 lightProjectionMatrix;
    glm::mat4 lightViewMatrix;

    // -- Eye's projection matrix & view matrix for rendering.
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    // Movement.
    glm::vec2 myMoveSpeed{0.0f, 0.0f};
    glm::vec2 myAngleSpeed{0.0f, 0.0f};
    glm::vec2 myPosition{-2.0f, 2.0f};
    glm::vec2 myAngle{glm::radians(150.0f), 0.3f};

    // Resolution.
    int resolutionSpeed = 0;

public:
    MyRenderer() : Engine::Renderer(INITIAL_WIDTH, INITIAL_HEIGHT, "Homework 3: 20130295 - Hunmin Park") {
        std::cout
                << "+-----------------------------+\n"
                << "| CS580 Homework Assignment 3 |\n"
                << "+-----------------------------+\n"
                << "\n";

        printKeymaps();

        // Initialize the models.
        // -- General models.
        jesusModel.setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.7f)));

        catModel1.setModelMatrix(multiplyMatrices(
                {
                        glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)),
                        glm::scale(glm::mat4(1.0f), glm::vec3(2.0f))
                }
        ));

        catModel2.setModelMatrix(multiplyMatrices(
                {
                        glm::translate(glm::vec3(0.0f, 0.0f, 3.0f)),
                        glm::scale(glm::mat4(1.0f), glm::vec3(2.0f))
                }
        ));

        chopperModel.setModelMatrix(multiplyMatrices(
                {
                        glm::translate(glm::vec3(-3.0f, 1.0f, 0.0f)),
                        glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                        glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
                        glm::scale(glm::mat4(1.0f), glm::vec3(0.1f))
                }
        ));

        myModel.setTexture(&catLightTexture);
        lightModel.setTexture(&lightTexture);
        skyModel.setTexture(&skyTexture);
        landModel.setTexture(&landTexture);
        jesusModel.setTexture(&jesusTexture);
        catModel1.setTexture(&catDarkTexture);
        catModel2.setTexture(&catLightTexture);
        chopperModel.setTexture(&chopperTexture);

        for (auto model: drawModelGroup) {
            model->setBrushTexture(&brushTexture);
            model->setShadowMap(depthFrameBuffer.getDepthTexture());
            model->setLight(0, backgroundLight);
            model->setLight(1, mainLight);
        }

        // -- Display model.
        displayModel.setTexture(drawFrameBuffer.getColorTexture());
        displayModel.setDepthMap(drawFrameBuffer.getDepthTexture());
        displayModel.setProgram(&displayProgram);

        // -- Select 0th model at the start.
        selectModelGroup[selectedModelIndex]->select(true);

        // OpenGL settings.
        // -- Depth test.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // -- Backface culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

private:
    void onDraw() override {
        // Change the resolution.
        displayModel.setResolution(displayModel.getResolution() + resolutionSpeed);

        // Rotate the main light.
        mainLight.position = glm::rotate(mainLight.position, 0.002f, glm::vec3(0.0f, 1.0f, 0.0f));

        lightModel.setModelMatrix(glm::translate(glm::scale(glm::vec3(0.5f)), mainLight.position));

        lightViewMatrix = glm::lookAt(
                mainLight.position,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        );

        for (auto model: drawModelGroup) {
            model->setLight(1, mainLight);
            model->setLightViewMatrix(lightViewMatrix);
        }

        // Move & rotate the camera.
        // -- Rotate the camera.
        float yAngleLimit = glm::radians(60.0f);

        myAngle += myAngleSpeed;

        if (myAngle.y > yAngleLimit) {
            myAngle.y = yAngleLimit;
        }

        if (myAngle.y < -yAngleLimit) {
            myAngle.y = -yAngleLimit;
        }

        glm::vec3 cameraDirection = glm::rotate(
                glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), myAngle.y, glm::vec3(1.0f, 0.0f, 0.0f)),
                myAngle.x,
                glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // -- Move the camera.
        glm::vec3 moveDirection = glm::normalize(cameraDirection);

        myPosition += glm::vec2(moveDirection.x, moveDirection.z) * myMoveSpeed.y
                      + glm::vec2(moveDirection.z, -moveDirection.x) * myMoveSpeed.x;

        glm::vec3 cameraPosition = glm::vec3(myPosition.x, 2.0f, myPosition.y);

        for (auto model: drawModelGroup) {
            model->setCameraPosition(cameraPosition);
        }

        // -- Let my character follow the camera.
        myModel.setModelMatrix(multiplyMatrices(
                {
                        glm::translate(glm::vec3(cameraPosition.x, 0.0f, cameraPosition.z)),
                        glm::rotate(glm::mat4(1.0f), myAngle.x, glm::vec3(0.0f, 1.0f, 0.0f))
                }
        ));

        // -- Update the view matrix.
        viewMatrix = glm::lookAt(
                cameraPosition,
                cameraPosition + cameraDirection,
                glm::vec3(0.0f, 1.0f, 0.0f)
        );

        for (auto model: drawModelGroup) {
            model->setViewMatrix(viewMatrix);
        }

        // Render.
        // -- First pass: Create the shadow map.
        depthFrameBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        for (auto model: shadowModelGroup) {
            model->setProgram(&depthProgram);
            model->draw();
        }

        depthFrameBuffer.unbind();

        // -- Second pass: Render the models on the frame buffer.
        drawFrameBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

        for (auto model: drawModelGroup) {
            model->setProgram(&drawProgram);
            model->draw();
        }

        drawFrameBuffer.unbind();

        // -- Third pass: Render the frame buffer on a rectangle.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        displayModel.draw();
    }

    void onSizeChange(int width, int height) override {
        // Resize the viewport.
        glViewport(0, 0, width, height);

        // Resize the frame buffers.
        drawFrameBuffer.setSize(width, height);
        depthFrameBuffer.setSize(width, height);

        // Reset the projection matrices.
        projectionMatrix = glm::perspective(
                90.0f,
                static_cast<GLfloat>(width) / static_cast<GLfloat>(height),
                0.5f,
                100.0f
        );

        lightProjectionMatrix = glm::ortho(
                -10.0f, 10.0f,
                -10.0f, 10.0f,
                -20.0f, 20.0f
        );

        for (auto model: drawModelGroup) {
            model->setProjectionMatrix(projectionMatrix);
            model->setLightProjectionMatrix(lightProjectionMatrix);
        }
    }

    void onKeyPress(int key) override {
        float v = 0.006f;
        float a = 0.001f;
        int r = 2;

        switch (key) {
        case GLFW_KEY_H:
            printKeymaps();
            break;
        case GLFW_KEY_R:
            // Select the next model.
            selectModelGroup[selectedModelIndex]->select(false);
            selectedModelIndex = static_cast<int>((selectedModelIndex + 1) % selectModelGroup.size());
            selectModelGroup[selectedModelIndex]->select(true);

            break;
        case GLFW_KEY_W:
            // Move front.
            myMoveSpeed.y = v;
            break;
        case GLFW_KEY_S:
            // Move back.
            myMoveSpeed.y = -v;
            break;
        case GLFW_KEY_A:
            // Move left.
            myMoveSpeed.x = v;
            break;
        case GLFW_KEY_D:
            // Move right.
            myMoveSpeed.x = -v;
            break;
        case GLFW_KEY_Q:
            // See left.
            myAngleSpeed.x = v;
            break;
        case GLFW_KEY_E:
            // See right.
            myAngleSpeed.x = -v;
            break;
        case GLFW_KEY_O:
            // See up.
            myAngleSpeed.y = -v;
            break;
        case GLFW_KEY_P:
            // See down.
            myAngleSpeed.y = v;
            break;
        case GLFW_KEY_U:
            // Higher resolution.
            resolutionSpeed = r;
            break;
        case GLFW_KEY_I:
            // Lower resolution.
            resolutionSpeed = -r;
            break;
        default:
            break;
        }
    }

    void onKeyRelease(int key) override {
        switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_S:
            myMoveSpeed.y = 0.0f;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_D:
            myMoveSpeed.x = 0.0f;
            break;
        case GLFW_KEY_Q:
        case GLFW_KEY_E:
            myAngleSpeed.x = 0.0f;
            break;
        case GLFW_KEY_O:
        case GLFW_KEY_P:
            myAngleSpeed.y = 0.0f;
            break;
        case GLFW_KEY_U:
        case GLFW_KEY_I:
            resolutionSpeed = 0;
            break;
        default:
            break;
        }
    }

private:
    void printKeymaps() {
        std::cout
                << "\n"
                << "- H(h): Print the keymaps.\n"
                << "- R(r): Select the next model.\n"
                << "- W(w) / S(s): Move front / back.\n"
                << "- A(a) / D(d): Move left / right.\n"
                << "- Q(q) / E(e): See left / right.\n"
                << "- O(o) / P(p): See up / down.\n"
                << "- U(u) / I(i): Increase / Decrease the resolution.\n";
    }

    // Since CLion can't detect GLM's operator overloading well, I made this function...
    glm::mat4 multiplyMatrices(std::initializer_list<glm::mat4> matrixList) {
        glm::mat4 result{1.0f};

        for (auto &matrix: matrixList) {
            result *= matrix;
        }

        return result;
    }
};

int main() {
    try {
        MyRenderer().run();

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error &error) {
        std::cout << error.what() << "\n";
        std::cin.get();

        return EXIT_FAILURE;
    }
}

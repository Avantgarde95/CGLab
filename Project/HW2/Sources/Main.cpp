#include "Engine/Engine.hpp"
#include "App/App.hpp"

class Scene : public Engine::Window {
public:
    // Shaders.
    std::string shaderPath = "Resources/Shaders/";
    Engine::Shader defaultShader{ shaderPath + "v-default.vert", shaderPath + "f-default.frag" };
    Engine::Shader blurShader{ shaderPath + "v-default.vert", shaderPath + "f-blur.frag" };
    Engine::Shader lightingShader{ shaderPath + "v-default.vert", shaderPath + "f-lighting.frag" };

    // Textures.
    std::string imagePath = "Resources/Images/";
    Engine::Texture wallTexture{ imagePath + "i-lab.ppm" };
    Engine::Texture wallNormalMap{ imagePath + "n-rocky.ppm" };
    Engine::Texture mobileTexture{ imagePath + "i-blank.ppm" };
    Engine::Texture mobileNormalMap{ imagePath + "n-blank.ppm" };

    // FBOs.
    Engine::FBO fbo;

    // Materials.
    Engine::Material defaultMaterial{
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        40.0f
    };

    // Lights.
    Engine::Light directionalLight{
        0,
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.3f, 0.2f, 0.2f),
        glm::vec3(0.3f, 0.2f, 0.2f),
        glm::vec3(0.3f, 0.2f, 0.2f),
        0.0f,
        0.0f
    };

    Engine::Light pointLight{
        1,
        glm::vec3(0.2f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        1.0f
    };

    Engine::Light spotLight{
        2,
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        20.0f,
        0.1f
    };

    // Models.
    App::DisplayModel displayModel{ 1.8f, -0.9f, -0.9f };
    App::WallModel wallModel{ 2.0f, -1.0f, -1.0f, -1.0f };
    std::vector<App::MobileNodeModel> mobileModel{ 7 }; // 0: root node

    // Global matrices.
    glm::mat4 projectionMatrix;

    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(1.0f, 1.2f, 1.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Extras.
    int currNodeIndex = 0;
    float pointLightArcsin = 0.0f;
    float spotLightAngle = 0.0f;
    bool enableBlur = false;

    Scene() : Engine::Window(600, 400, "Homework 2: 20130295 - Hunmin Park") {
        std::cout
            << "+-----------------------------+\n"
            << "| CS580 Homework Assignment 2 |\n"
            << "+-----------------------------+\n";

        printKeymaps();
    }

    void onStart() override {
        // Depth test.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Create the shaders.
        defaultShader.create();
        lightingShader.create();
        blurShader.create();

        // Create the textures.
        wallTexture.create();
        wallNormalMap.create();
        mobileTexture.create();
        mobileNormalMap.create();

        // Create the FBOs.
        fbo.create();

        // Create the models.
        // -- displayModel.
        displayModel.setShader(defaultShader);
        displayModel.create();

        // -- wallModel.
        wallModel.setShader(lightingShader);
        wallModel.setTexture(wallTexture);
        wallModel.setNormalMap(wallNormalMap);
        wallModel.setLight(0, directionalLight);
        wallModel.setLight(1, pointLight);
        wallModel.setLight(2, spotLight);
        wallModel.setMaterial(defaultMaterial);
        wallModel.setProjectionMatrix(projectionMatrix);
        wallModel.setViewMatrix(viewMatrix);
        wallModel.create();

        // -- mobileModel.
        buildMobile();

        mobileModel[0].setShader(lightingShader);
        mobileModel[0].setTexture(mobileTexture);
        mobileModel[0].setNormalMap(mobileNormalMap);
        mobileModel[0].setLight(0, directionalLight);
        mobileModel[0].setLight(1, pointLight);
        mobileModel[0].setLight(2, spotLight);
        mobileModel[0].setMaterial(defaultMaterial);
        mobileModel[0].setProjectionMatrix(projectionMatrix);
        mobileModel[0].setViewMatrix(viewMatrix);
        mobileModel[0].create();

        mobileModel[currNodeIndex].setFill(false);
    }

    void onDraw() override {
        movePointLight();
        rotateSpotLight();

        // Draw the models on the FBO.
        fbo.bind();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        wallModel.draw();
        mobileModel[0].draw();
        fbo.unbind();

        // Draw the FBO on displayModel.
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        displayModel.setTexture(fbo);
        displayModel.draw();
    }

    void onSizeChange(int width, int height) override {
        glViewport(0, 0, width, height);
        fbo.setSize(width, height);

        projectionMatrix = glm::perspective(
            45.0f,
            static_cast<GLfloat>(width) / static_cast<GLfloat>(height),
            0.5f,
            50.0f
        );
    }

    void onKeyPress(int key) override {
        switch (key) {
        case GLFW_KEY_H:
            printKeymaps();
            break;
        case GLFW_KEY_O:
            selectNextNode();
            break;
        case GLFW_KEY_M:
            increaseRotationSpeed();
            break;
        case GLFW_KEY_N:
            decreaseRotationSpeed();
            break;
        case GLFW_KEY_R:
            resetRotationSpeed();
            break;
        case GLFW_KEY_B:
            toggleBlur();
            break;
        }
    }

private:
    void buildMobile() {
        // 0 (= Root).
        mobileModel[0].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("color", glm::vec3(0.5f, 0.5f, 0.5f))
            .buildCylinder()
        );

        mobileModel[0].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[0].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[0].setTranslation(glm::vec3(0.0f, 0.6f, 0.0f));

        // 0 -- 1.
        mobileModel[1].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.07f)
            .setAttribute("color", glm::vec3(0.0f, 0.3f, 0.3f))
            .setMatrix(glm::translate(glm::vec3(0.0f, 0.05f, 0.0f)))
            .buildCylinder()
        );

        mobileModel[1].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("color", glm::vec3(0.0f, 0.3f, 0.3f))
            .setMatrix(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)))
            .buildCylinder()
        );

        mobileModel[1].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.07f)
            .setAttribute("color", glm::vec3(0.0f, 0.3f, 0.3f))
            .setMatrix(glm::translate(glm::vec3(0.0f, -0.05f, 0.0f)))
            .buildCylinder()
        );

        mobileModel[1].setTopPosition(glm::vec3(0.0f, 0.1f, 0.0f));
        mobileModel[1].setBottomPosition(glm::vec3(0.0f, -0.1f, 0.0f));
        mobileModel[1].setTranslation(glm::vec3(0.4f, -0.3f, 0.0f));

        mobileModel[0].addChild(mobileModel[1]);

        // 0 -- 1 -- 2.
        mobileModel[2].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 4)
            .setAttribute("color", glm::vec3(1.0f, 1.0f, 1.0f))
            .buildPrism()
        );

        mobileModel[2].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[2].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[2].setTranslation(glm::vec3(0.3f, -0.3f, 0.0f));
        mobileModel[1].addChild(mobileModel[2]);

        // 0 -- 1 -- 3.
        mobileModel[3].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 3)
            .setAttribute("color", glm::vec3(0.2f, 0.7f, 0.2f))
            .buildPyramid()
        );

        mobileModel[3].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[3].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[3].setTranslation(glm::vec3(-0.15f, -0.3f, 0.26f));
        mobileModel[1].addChild(mobileModel[3]);

        // 0 -- 1 -- 4.
        mobileModel[4].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 5)
            .setAttribute("color", glm::vec3(0.7f, 0.7f, 0.2f))
            .buildPyramid()
        );

        mobileModel[4].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[4].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[4].setTranslation(glm::vec3(-0.15f, -0.3f, -0.26f));
        mobileModel[1].addChild(mobileModel[4]);

        // 0 -- 5.
        mobileModel[5].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 5)
            .setAttribute("color", glm::vec3(0.5f, 0.2f, 0.2f))
            .buildPyramid()
        );

        mobileModel[5].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[5].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[5].setTranslation(glm::vec3(-0.4f, -0.3f, 0.0f));

        mobileModel[0].addChild(mobileModel[5]);

        // 0 -- 5 -- 6.
        mobileModel[6].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 5)
            .setAttribute("color", glm::vec3(0.5f, 0.2f, 0.2f))
            .buildPyramid()
        );

        mobileModel[6].addShape(
            App::Shape()
            .setAttribute("height", 0.1f)
            .setAttribute("radius", 0.1f)
            .setAttribute("level", 5)
            .setAttribute("color", glm::vec3(0.5f, 0.2f, 0.2f))
            .setMatrix(glm::translate(glm::vec3(0.0f, -0.1f, 0.0f)))
            .buildPrism()
        );

        mobileModel[6].setTopPosition(glm::vec3(0.0f, 0.05f, 0.0f));
        mobileModel[6].setBottomPosition(glm::vec3(0.0f, -0.05f, 0.0f));
        mobileModel[6].setTranslation(glm::vec3(0.0f, -0.3f, 0.0f));

        mobileModel[5].addChild(mobileModel[6]);

    }

    void printKeymaps() {
        std::cout
            << "\n"
            << "- H(h): Print the keymaps.\n"
            << "- O(o): Select the next node.\n"
            << "- M(m): Increase the speed of the current node.\n"
            << "- N(n): Decrease the speed of the current node.\n"
            << "- R(r): Reset the speed of the current node.\n"
            << "- B(b): Blurring on / off.\n"
            << "\n";
    }

    void selectNextNode() {
        mobileModel[currNodeIndex].setFill(true);
        currNodeIndex = (currNodeIndex + 1) % mobileModel.size();
        std::cout << "Select the node: " << currNodeIndex << "\n";
        mobileModel[currNodeIndex].setFill(false);
    }

    void increaseRotationSpeed() {
        std::cout << "Increase the speed of the current node.\n";
        mobileModel[currNodeIndex].addRotationSpeed(0.001f);
    }

    void decreaseRotationSpeed() {
        std::cout << "Decrease the speed of the current node.\n";
        mobileModel[currNodeIndex].addRotationSpeed(-0.001f);
    }

    void resetRotationSpeed() {
        std::cout << "Reset the speed of the current node.\n";
        mobileModel[currNodeIndex].resetRotationSpeed();
    }

    void toggleBlur() {
        enableBlur = !enableBlur;
        std::cout << (enableBlur ? "Blur on.\n" : "Blur off.\n");
        displayModel.setShader(enableBlur ? blurShader : defaultShader);
    }

    void movePointLight() {
        float tau = 3.141592f * 2.0f;
        pointLightArcsin += 0.007f;

        if (pointLightArcsin > tau) {
            pointLightArcsin -= tau;
        }

        pointLight.position.y = 0.3f + glm::sin(pointLightArcsin);

        wallModel.setLight(1, pointLight);
        mobileModel[0].setLight(1, pointLight);
    }

    void rotateSpotLight() {
        float tau = 3.141592f * 2.0f;

        spotLightAngle += 0.005f;

        if (spotLightAngle > tau) {
            spotLightAngle -= tau;
        }

        glm::vec3 target{ 0.3f * glm::cos(spotLightAngle), 0.0f, 0.3f * glm::sin(spotLightAngle) };
        spotLight.direction = target - spotLight.position;

        wallModel.setLight(2, spotLight);
        mobileModel[0].setLight(2, spotLight);
    }
};

int main() {
    return Scene().run();
}

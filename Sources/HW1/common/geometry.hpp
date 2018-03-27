#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
//#include <common/model.hpp>

// Class for calculating the frame rate.
class Timer {
private:
    // Last glfwGetTime().
    float lastTime = 0;

    // Number of the frames per second.
    int frameCount = 0;
public:
    // Tick the timer.
    void tick();

    // Getter: frameCount
    int getFrameCount();
};

// Class for building a primitive.
class Shape {
private:
    // Matrix for positioning the primitive.
    // (This matrix should NOT be used for transformation.)
    glm::mat4 matrix;

    // Scalar attributes (ex. height, radius) and vector attributes (ex. color)
    std::map<std::string, float> valueAttributeMap;
    std::map<std::string, glm::vec3> vectorAttributeMap;

    // Vertices information
    std::vector<glm::vec3> positionList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec3> colorList;
public:
    // Setter: matrix
    Shape & setMatrix(glm::mat4 matrix);

    // Setter: valueAttributeMap, vectorAttributeMap
    Shape & setAttribute(std::string key, float value);
    Shape & setAttribute(std::string key, glm::vec3 value);

    // Build the primitives (using the attributes)
    Shape & buildPyramid();
    Shape & buildPrism();
    Shape & buildCone();
    Shape & buildCylinder();
private:
    // Build a triangle and update xxxList members.
    void addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color);
    friend class Object;
};

// Class which represents a single model.
// (I got most of the code from common/model.cpp.)
class Object {
protected:
    // Whether we draw the whole object or only its wireframe.
    bool fill = true;

    // OpenGL drawing mode. (ex. GL_TRIANGLES, GL_LINES, etc.)
    GLint drawMode = GL_TRIANGLES;

    // Model matrix.
    glm::mat4 modelMatrix;

    // Vertices information.
    std::vector<glm::vec3> positionList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec3> colorList;

    // Members for communicating with the shaders.
    GLuint programId;
    GLuint vertexArrayId;
    GLuint positionBufferId;
    GLuint normalBufferId;
    GLuint colorBufferId;
    GLuint positionBufferIndex = 0;
    GLuint normalBufferIndex = 1;
    GLuint colorBufferIndex = 2;
public:
    // Destructor: Clean the members before the model is deleted.
    ~Object();

    // Add a primitive to the model.
    Object & addShape(Shape shape);

    // Setter: fill
    Object & setFill(bool fill);

    // Initialize the shaders and build the model.
    void create(std::string vertexShaderPath, std::string fragmentShaderPath);

    // Draw the model.
    void draw(glm::mat4& projectionMatrix, glm::mat4& eyeMatrix, glm::vec3& lightDirection);
private:
    // Helper functions for create() and draw()
    void createArrayBuffer(std::vector<glm::vec3>& data, GLuint& id);
    void bindArrayBuffer(GLuint& id, GLuint& index);
    void sendVec3ToShaders(glm::vec3& v, const char* name);
    void sendMat4ToShaders(glm::mat4& m, const char* name);
};

// Connecting line between two nodes.
class MobileConnector : public Object {
public:
    // Constructor
    MobileConnector();

    // Override
    void draw(glm::mat4& projectionMatrix, glm::mat4& eyeMatrix, glm::vec3& lightDirection);

    friend class MobileNode;
};

// Each node of the mobile.
class MobileNode : public Object {
private:
    // Translation w.r.t the parent node.
    glm::vec3 translation;

    // Matrix for handling rotation.
    // (At each frame, instead of increasing the rotation angle,
    //  we just rotate rotationMatrix.)
    glm::mat4 rotationMatrix;

    // Rotation speed.
    float rotationSpeed = 0;

    // Reference to the parent node.
    MobileNode* parent = nullptr;

    // Top / bottom vertices. (Used for drawing the connector.)
    glm::vec3 topVertex;
    glm::vec3 bottomVertex;

    // A line which connects this node and the parent node.
    MobileConnector connector;

    // List of the child nodes.
    std::vector<MobileNode*> childList;
public:
    // Override
    void create(std::string vertexShaderPath, std::string fragmentShaderPath);

    // Override
    MobileNode & addShape(Shape shape);
    MobileNode & setFill(bool fill);

    // Add a child node to childList.
    MobileNode & addChild(MobileNode* child);
    MobileNode & addChild(MobileNode& child);

    // Setter: translation / rotationSpeed / topVertex / bottomVertex
    MobileNode & setTranslation(glm::vec3 translation);
    MobileNode & setRotationSpeed(float rotationSpeed);
    MobileNode & setTopVertex(glm::vec3 topVertex);
    MobileNode & setBottomVertex(glm::vec3 bottomVertex);

    // Getter: rotationSpeed
    float getRotationSpeed();

    // Override: Draw this node and the child nodes.
    void draw(glm::mat4& projectionMatrix, glm::mat4& eyeMatrix, glm::vec3& lightDirection);
};

// Whole mobile: Class for managing the nodes.
class Mobile {
private:
    // List of the nodes.
    std::vector<MobileNode> nodeList;

    // Index of the current node.
    int currIndex;
public:
    // Constructor
    Mobile(int size);

    // Get the index-th node.
    MobileNode & getNode(int index);
    MobileNode & operator[](int index);

    // Get currently selected node.
    MobileNode & getCurrentNode();

    // Select the next node.
    void selectNextNode();

    // Call create() for all nodes.
    void create();
};

// ==================================================================

void Timer::tick() {
    float currTime = glfwGetTime();
    frameCount++;

    if (currTime - lastTime >= 1.0f) {
        frameCount = 0;
        lastTime += 1.0f;
    }
}

int Timer::getFrameCount() {
    return frameCount;
}

// ==================================================================

Shape& Shape::setMatrix(glm::mat4 matrix) {
    this->matrix = matrix;
    return *this;
}

Shape& Shape::setAttribute(std::string key, float value) {
    valueAttributeMap[key] = value;
    return *this;
}

Shape& Shape::setAttribute(std::string key, glm::vec3 value) {
    vectorAttributeMap[key] = value;
    return *this;
}

Shape& Shape::buildPyramid() {
    float height = valueAttributeMap["height"];
    float radius = valueAttributeMap["radius"];
    int level = valueAttributeMap["level"];
    glm::vec3 color = vectorAttributeMap["color"];

    glm::vec3 apex(0, height / 2.0f, 0);
    glm::vec3 baseCenter(0, -height / 2.0f, 0);
    std::vector<glm::vec3> baseVertices;
    float angle = (2.0f * 3.141592f) / level;

    for (int i = 0; i < level + 1; i++) {
        baseVertices.push_back(glm::vec3(
            baseCenter.x + radius * std::cos(angle * i),
            baseCenter.y,
            baseCenter.z + radius * std::sin(angle * i)
        ));
    }

    for (int i = 0; i < level; i++) {
        glm::vec3 v0 = baseVertices[i];
        glm::vec3 v1 = baseVertices[i + 1];

        addTriangle(v0, v1, apex, color);
        addTriangle(v1, v0, baseCenter, color);
    }

    return *this;
}

Shape& Shape::buildPrism() {
    float height = valueAttributeMap["height"];
    float radius = valueAttributeMap["radius"];
    int level = valueAttributeMap["level"];
    glm::vec3 color = vectorAttributeMap["color"];

    glm::vec3 topCenter(0, height / 2.0f, 0);
    glm::vec3 bottomCenter(0, -height / 2.0f, 0);
    std::vector<glm::vec3> topVertices;
    std::vector<glm::vec3> bottomVertices;
    float angle = (2.0f * 3.141592f) / level;

    for (int i = 0; i < level + 1; i++) {
        topVertices.push_back(glm::vec3(
            topCenter.x + radius * std::cos(angle * i),
            topCenter.y,
            topCenter.z + radius * std::sin(angle * i)
        ));

        bottomVertices.push_back(glm::vec3(
            bottomCenter.x + radius * std::cos(angle * i),
            bottomCenter.y,
            bottomCenter.z + radius * std::sin(angle * i)
        ));
    }

    for (int i = 0; i < level; i++) {
        glm::vec3 v0 = topVertices[i];
        glm::vec3 v1 = topVertices[i + 1];
        glm::vec3 w0 = bottomVertices[i];
        glm::vec3 w1 = bottomVertices[i + 1];

        addTriangle(v0, v1, topCenter, color);
        addTriangle(v0, w0, w1, color);
        addTriangle(w1, v1, v0, color);
        addTriangle(w1, w0, bottomCenter, color);
    }

    return *this;
}

Shape& Shape::buildCone() {
    return setAttribute("level", 30).buildPyramid();
}

Shape& Shape::buildCylinder() {
    return setAttribute("level", 30).buildPrism();
}

void Shape::addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color) {
    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

    positionList.push_back(v0);
    positionList.push_back(v1);
    positionList.push_back(v2);

    colorList.push_back(color);
    colorList.push_back(color);
    colorList.push_back(color);

    normalList.push_back(normal);
    normalList.push_back(normal);
    normalList.push_back(normal);
}

// ==================================================================

Object::~Object() {
    glDisableVertexAttribArray(positionBufferIndex);
    glDisableVertexAttribArray(normalBufferIndex);
    glDisableVertexAttribArray(colorBufferIndex);

    glDeleteBuffers(1, &positionBufferId);
    glDeleteBuffers(1, &normalBufferId);
    glDeleteBuffers(1, &colorBufferId);

    glDeleteProgram(programId);
    glDeleteVertexArrays(1, &vertexArrayId);
}

Object& Object::addShape(Shape shape) {
    auto vertexMatrix = shape.matrix;
    auto normalMatrix = glm::transpose(glm::inverse(shape.matrix));

    for (auto it = shape.positionList.begin(); it != shape.positionList.end(); it++) {
        this->positionList.push_back(glm::vec3(vertexMatrix * glm::vec4(*it, 1)));
    }

    for (auto it = shape.normalList.begin(); it != shape.normalList.end(); it++) {
        this->normalList.push_back(glm::vec3(normalMatrix * glm::vec4(*it, 1)));
    }

    for (auto it = shape.colorList.begin(); it != shape.colorList.end(); it++) {
        this->colorList.push_back(*it);
    }

    return *this;
}

Object& Object::setFill(bool fill) {
    this->fill = fill;
    return *this;
}

void Object::create(std::string vertexShaderPath, std::string fragmentShaderPath) {
    programId = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    createArrayBuffer(positionList, positionBufferId);
    createArrayBuffer(normalList, normalBufferId);
    createArrayBuffer(colorList, colorBufferId);
}

void Object::draw(glm::mat4& projectionMatrix, glm::mat4& eyeMatrix, glm::vec3& lightDirection) {
    glUseProgram(programId);

    sendVec3ToShaders(lightDirection, "uLight");
    sendMat4ToShaders(projectionMatrix, "Projection");
    sendMat4ToShaders(eyeMatrix, "Eye");
    sendMat4ToShaders(modelMatrix, "ModelTransform");

    glBindVertexArray(vertexArrayId);

    bindArrayBuffer(positionBufferId, positionBufferIndex);
    bindArrayBuffer(normalBufferId, normalBufferIndex);
    bindArrayBuffer(colorBufferId, colorBufferIndex);

    glPolygonMode(GL_FRONT_AND_BACK, (fill ? GL_FILL : GL_LINE));
    glDrawArrays(drawMode, 0, static_cast<GLsizei>(positionList.size()));
}

void Object::createArrayBuffer(std::vector<glm::vec3>& data, GLuint& id) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);

    glBufferData(
        GL_ARRAY_BUFFER,                 // target
        data.size() * sizeof(glm::vec3), // size
        &(data[0]),                      // data
        GL_STATIC_DRAW                   // usage
    );
}

void Object::bindArrayBuffer(GLuint& id, GLuint& index) {
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, id);

    glVertexAttribPointer(
        index,                  // index
        3,                      // size
        GL_FLOAT,               // type
        GL_FALSE,               // normalized
        sizeof(glm::vec3),      // stride
        static_cast<GLvoid*>(0) // pointer
    );
}

void Object::sendVec3ToShaders(glm::vec3& v, const char* name) {
    glUniform3f(
        glGetUniformLocation(programId, name), // location
        v.x, v.y, v.z                          // v0, v1, v2
    );
}

void Object::sendMat4ToShaders(glm::mat4& m, const char* name) {
    glUniformMatrix4fv(
        glGetUniformLocation(programId, name), // location
        1,                                     // count
        GL_FALSE,                              // transpose
        &(m[0][0])                             // value
    );
}
// ==================================================================

MobileConnector::MobileConnector() {
    drawMode = GL_LINES;

    positionList.push_back(glm::vec3(0, 0, 0));
    positionList.push_back(glm::vec3(0, 0, 0));
    normalList.push_back(glm::vec3(0, 0, 1));
    normalList.push_back(glm::vec3(0, 0, 1));
    colorList.push_back(glm::vec3(1, 1, 1));
    colorList.push_back(glm::vec3(1, 1, 1));
}

void MobileConnector::draw(glm::mat4& projectionMatrix, glm::mat4& eyeMatrix, glm::vec3& lightDirection) {
    // re-bind positionList to apply the changes
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);

    glBufferData(
        GL_ARRAY_BUFFER,
        positionList.size() * sizeof(glm::vec3),
        &(positionList[0]),
        GL_STATIC_DRAW
    );

    Object::draw(projectionMatrix, eyeMatrix, lightDirection);
}

// ==================================================================

void MobileNode::create(std::string vertexShaderPath, std::string fragmentShaderPath) {
    Object::create(vertexShaderPath, fragmentShaderPath);
    connector.create(vertexShaderPath, fragmentShaderPath);
}

MobileNode & MobileNode::addShape(Shape shape) {
    Object::addShape(shape);
    return *this;
}

MobileNode & MobileNode::setFill(bool fill) {
    Object::setFill(fill);
    return *this;
}

MobileNode& MobileNode::addChild(MobileNode* child) {
    childList.push_back(child);
    child->parent = this;
    return *this;
}

MobileNode& MobileNode::addChild(MobileNode& child) {
    return addChild(&child);
}

MobileNode & MobileNode::setTranslation(glm::vec3 translation) {
    this->translation = translation;
    return *this;
}

MobileNode & MobileNode::setRotationSpeed(float rotationSpeed) {
    this->rotationSpeed = rotationSpeed;
    return *this;
}

MobileNode & MobileNode::setTopVertex(glm::vec3 topVertex) {
    this->topVertex = topVertex;
    return *this;
}

MobileNode & MobileNode::setBottomVertex(glm::vec3 bottomVertex) {
    this->bottomVertex = bottomVertex;
    return *this;
}

float MobileNode::getRotationSpeed() {
    return rotationSpeed;
}

void MobileNode::draw(glm::mat4 & projectionMatrix, glm::mat4 & eyeMatrix, glm::vec3 & lightDirection) {
    // move the node upward a little bit ("realistic physics simulation!")
    // (I know that this actually looks unrealistic... but this was the best algorithm I could implement.)
    auto realTranslation = translation;

    if (parent != nullptr) {
        auto moveFactor = 0.5f * glm::abs(parent->rotationSpeed);
        realTranslation.x *= 1.0f + moveFactor;
        realTranslation.z *= 1.0f + moveFactor;
        realTranslation.y = glm::min(0.0f, realTranslation.y + moveFactor);
    }

    // apply the transformations to the node
    rotationMatrix = glm::rotate(rotationMatrix, rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(realTranslation) * rotationMatrix;

    // apply the parent's transformation to the node
    if (parent != nullptr) {
        this->modelMatrix = (parent->modelMatrix) * (this->modelMatrix);
    }

    // draw this node
    Object::draw(projectionMatrix, eyeMatrix, lightDirection);

    // draw the connector
    if (parent != nullptr) {
        connector.positionList[0] = glm::vec3((parent->modelMatrix) * glm::vec4(parent->bottomVertex, 1));
        connector.positionList[1] = glm::vec3((this->modelMatrix) * glm::vec4(this->topVertex, 1));
        // make the connector always bright by setting the light toward the connector
        glm::vec3 connectorLightDirection = glm::vec3((this->modelMatrix) * glm::vec4(0, 0, -1, 1));
        connector.draw(projectionMatrix, eyeMatrix, connectorLightDirection);
    }

    // draw the child nodes
    for (auto it = childList.begin(); it != childList.end(); it++) {
        (*it)->draw(projectionMatrix, eyeMatrix, lightDirection);
    }
}

// ==================================================================

Mobile::Mobile(int size) {
    nodeList.resize(size);
    currIndex = nodeList.size() - 1;
    selectNextNode();
}

MobileNode& Mobile::getNode(int index) {
    return nodeList[index];
}

MobileNode& Mobile::operator[](int index) {
    return getNode(index);
}

MobileNode& Mobile::getCurrentNode() {
    return nodeList[currIndex];
}

void Mobile::selectNextNode() {
    nodeList[currIndex].setFill(true);
    currIndex = (currIndex + 1) % nodeList.size();
    std::printf("Selecting %02dth node\n", currIndex);
    nodeList[currIndex].setFill(false);
}

void Mobile::create() {
    for (auto it = nodeList.begin(); it != nodeList.end(); it++) {
        std::printf("Creating %02dth node\n\n", static_cast<int>(it - nodeList.begin()));
        it->create("VertexShader.glsl", "FragmentShader.glsl");
    }
}

#endif

#include "Cube.h"

Cube::Cube(Shader* shd) : Mesh(shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Cube(Shader* shd).");
    }
    primitiveCount = 12;
    primitives = {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };

    worldMat = shd->getMat4Uniform("modelMatrix");
    color = Vector4f(1.f, 0.f, 0.f, 1.f);
}

void Cube::setPosition(float x, float y, float z) {
    position = Vector3f(x, y, z);
}

void Cube::addPositionXZ(const Vector2f& vect) {
    position.x += vect.x;
    position.z += vect.y;
}

void Cube::setScale(float x, float y, float z) {
    scale = Vector3f(x, y, z);
}

void Cube::addScale(float sca) {
    scale.add(Vector3f(sca, sca, sca));
}

void Cube::addRotationX(float bruh) {
    rotation.x += bruh;
}

void Cube::addRotationY(float bruh) {
    rotation.y += bruh;
}

void Cube::addRotationZ(float bruh) {
    rotation.z += bruh;
}

void Cube::generateData() {
    vertexData.clear();

    for (int i = 0; i < 24; i++) {
        vertexData.push_back(vertices[i]);
    }
}

void Cube::uploadData() {
    GLuint err = GL_NO_ERROR;

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to buffer vertex data!");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitives.size()*sizeof(GLuint), primitives.data(), GL_STATIC_DRAW);
}

void Cube::renderInternal() {
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(position, scale, rotation);
    worldMat->setValue(mat);
    shader->getVector4fUniform("fsColor")->setValue(color);
}

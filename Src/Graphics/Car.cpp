#include <cmath>

#include "Car.h"
#include "Cube.h"
#include "Shader.h"

Car::Car(Shader* shd) {
    renderingMode = GL_FILL;
    
    Cube* body = new Cube(shd);
    body->setScale(4.f, 0.5f, 6.f);
    body->setPosition(0.f, 0.5f, 0.f);
    Cube* leftWall = new Cube(shd);
    leftWall->setScale(0.5f, 1.25f, 6.f);
    leftWall->setPosition(-1.75f, 1.f, 0.f);
    Cube* rightWall = new Cube(shd);
    rightWall->setScale(0.5f, 1.25f, 6.f);
    rightWall->setPosition(1.75f, 1.f, 0.f);
    Cube* frontWall = new Cube(shd);
    frontWall->setScale(3.f, 1.25f, 0.5f);
    frontWall->setPosition(0.f, 1.f, 2.75f);
    Cube* backWall = new Cube(shd);
    backWall->setScale(3.f, 1.25f, 0.5f);
    backWall->setPosition(0.f, 1.f, -2.75f);
    
    wheels[0] = new Cube(shd);
    wheels[0]->setPosition(4.5f, 0.f, 6.5f);
    wheels[1] = new Cube(shd);
    wheels[1]->setPosition(4.5f, 0.f, -6.5f);
    wheels[2] = new Cube(shd);
    wheels[2]->setPosition(-4.5f, 0.f, 6.5f);
    wheels[3] = new Cube(shd);
    wheels[3]->setPosition(-4.5f, 0.f, -6.5f);
    
    for (int i = 0; i < 4; i++) {
        wheels[i]->color = Vector4f(0.f, 0.f, 0.f, 1.f);
    }

    cubes.push_back(body);
    cubes.push_back(leftWall);
    cubes.push_back(rightWall);
    cubes.push_back(frontWall);
    cubes.push_back(backWall);
    for (int i = 0; i < 4; i++) {
        cubes.push_back(wheels[i]);
    }
}

void Car::addPositionXZ(const Vector2f& vect) {
    position.x += vect.x;
    position.z += vect.y;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addPositionXZ(vect);
    }
}

void Car::setScale(float x, float y, float z) {
    scale = Vector3f(x, y, z);
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->setScale(x, y, z);
    }
}

void Car::addScale(float sca) {
    scale = scale.add(Vector3f(sca, sca, sca));
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addScale(sca);
    }
}

void Car::addRotationX(float bruh) {
    rotation.x += bruh;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addRotationX(bruh);
    }
}

void Car::addRotationY(float bruh) {
    rotation.y += bruh;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addRotationOriginY(bruh);
    }
}

void Car::addRotationZ(float bruh) {
    rotation.z += bruh;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addRotationZ(bruh);
    }
}

void Car::setRenderingMode(GLenum mode) {
    renderingMode = mode;
}

void Car::walk(Car::WalkInput input, float speed) {
    if (input == WalkInput::None) { return; }
    
    float sinAngle = std::sin(-rotation.y); // Invert this to use the correct coordinate system.
    float cosAngle = std::cos(rotation.y);
    
    Vector2f targetDir = Vector2f::zero;
    if ((input & WalkInput::Forward) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(sinAngle,cosAngle));
        // Rotate wheels.
        for (int i = 0; i < 4; i++) {
            wheels[i]->addRotationX(-speed);
        }
    }
    if ((input & WalkInput::Backward) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(-sinAngle,-cosAngle));
        // Rotate wheels.
        for (int i = 0; i < 4; i++) {
            wheels[i]->addRotationX(speed);
        }
    }
    if ((input & WalkInput::Left) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(-cosAngle,sinAngle));
    }
    if ((input & WalkInput::Right) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(cosAngle,-sinAngle));
    }
    
    if (targetDir.lengthSquared() < 0.01f) { return; }
    addPositionXZ(targetDir.normalize().multiply(speed));
}

void Car::render() {
    glPolygonMode(GL_FRONT_AND_BACK, renderingMode);
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->render(position);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const Car::WalkInput operator&(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a & (int)b);
}

const Car::WalkInput operator|(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a | (int)b);
}

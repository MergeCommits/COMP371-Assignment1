#include <cmath>

#include "Car.h"
#include "Cube.h"
#include "Shader.h"

Car::Car(Shader* shd) {
    Cube* body = new Cube(shd);
    body->setScale(4.f, 2.f, 6.f);
    body->setPosition(0.f, 0.5f, 0.f);
    
    wheels[0] = new Cube(shd);
    wheels[0]->setPosition(4.5f, 0.f, 6.5f);
    wheels[1] = new Cube(shd);
    wheels[1]->setPosition(4.5f, 0.f, -6.5f);
    wheels[2] = new Cube(shd);
    wheels[2]->setPosition(-4.5f, 0.f, 6.5f);
    wheels[3] = new Cube(shd);
    wheels[3]->setPosition(-4.5f, 0.f, -6.5f);

    cubes.push_back(body);
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
        cubes[i]->addRotationY(bruh);
    }
}

void Car::addRotationZ(float bruh) {
    rotation.z += bruh;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addRotationZ(bruh);
    }
}

void Car::walk(bool forward, float speed) {
    float sinAngle = std::sin(-rotation.y);
    float cosAngle = std::cos(-rotation.y);
    
    Vector2f targetDir = Vector2f::zero;
    if (forward) {
        targetDir = targetDir.add(Vector2f(sinAngle, cosAngle));
    } else {
        targetDir = targetDir.add(Vector2f(-sinAngle, -cosAngle));
    }
    addPositionXZ(targetDir.normalize().multiply(speed));
}

void Car::render() {
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->render();
    }
}

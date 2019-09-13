#include "Car.h"
#include "Cube.h"
#include "Shader.h"

Car::Car(Shader* shd) {
    Cube* body = new Cube(shd);
    body->setScale(4.f, 2.f, 1.f);
//    body->setPosition(0.f, 2.f, 0.f);

    cubes.push_back(body);
}

void Car::addPositionXY(const Vector2f& vect) {
    position.x += vect.x;
    position.y += vect.y;
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->addPositionXY(vect);
    }
}

void Car::setScale(float x, float y, float z) {
    scale = Vector3f(x, y, z);
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->setScale(x, y, z);
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

void Car::render() {
    for (int i = 0; i < (int)cubes.size(); i++) {
        cubes[i]->render();
    }
}

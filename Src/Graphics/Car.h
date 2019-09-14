#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include <vector>

#include "../Math/Vector.h"

class Cube;
class Shader;

class Car {
private:
    Vector3f position;
    Vector3f scale;
    Vector3f rotation;
    
    std::vector<Cube*> cubes;
    Cube* wheels[4];
    
public:
    Car(Shader* shd);
    
    void walk(bool forward, float speed);
    void addPositionXZ(const Vector2f& vect);
    void setScale(float x, float y, float z);
    void addScale(float sca);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    void addRotationZ(float bruh);
    
    void render();
};

#endif // CAR_H_INCLUDED

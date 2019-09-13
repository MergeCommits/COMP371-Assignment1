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
    
public:
    Car(Shader* shd);
    
    void addPositionXY(const Vector2f& vect);
    void setScale(float x, float y, float z);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    void addRotationZ(float bruh);
    
    void render();
};

#endif // CAR_H_INCLUDED

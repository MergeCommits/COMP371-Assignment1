#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../Math/Matrix.h"
#include "Shader.h"

class GraphicsResources;

class Camera {
public:
    enum class WalkInput {
        None = 0x0,
        Forward = 0x1,
        Backward = 0x2,
        Left = 0x4,
        Right = 0x8
    };
    
private:
    GraphicsResources* gfxRes;

    float xAngle;
    float yAngle;
    // The range of the the yAngle field before it is clamped.
    float yAngleLimit;
    float tilt;

    float aspectRatio;
    float fov;

    bool needsViewUpdate;
    bool needsProjUpdate;

    Vector3f position;
    Vector3f lookAt;
    Vector3f upDir;

    Matrix4x4f viewMatrix;
    Matrix4x4f projectionMatrix;
    Matrix4x4f rotation;

    std::vector<Shader*> shaders;

public:
    Camera(float aspectRatio, float fov);
    Camera(float aspectRatio);

    void addShader(Shader* shd);

    void update();
    void walk(WalkInput input, float speed);

    void setPosition(const Vector3f& pos);
    void setTilt(float rad);
    void addAngle(float xAngle, float yAngle);
    void resetAngle();
    void addFov(float deg);
};

const Camera::WalkInput operator&(const Camera::WalkInput& a, const Camera::WalkInput& b);
const Camera::WalkInput operator|(const Camera::WalkInput& a, const Camera::WalkInput& b);

#endif // CAMERA_H_INCLUDED

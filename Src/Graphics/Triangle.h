#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include <vector>

#include "Mesh.h"

class Triangle : public Mesh {
private:
    const float vertices[9] = {
        0.f, 0.5f, 0.f,    // Top.
        -0.5f, -0.5f, 0.f, // Left.
        0.5f, -0.5f, 0.f   // Right.
    };

    std::vector<GLuint> primitives;

    const float colors[12] = {
        1.f, 0.f, 0.f, 1.f, // Red.
        0.f, 0.f, 1.f, 1.f, // Blue.
        0.f, 1.f, 0.f, 1.f  // Green.
    };

    std::vector<float> vertexData;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;

public:
    Triangle(Shader* shd);
};

#endif // TRIANGLE_H_INCLUDED

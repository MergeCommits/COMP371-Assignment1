#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>

#include "Mesh.h"
#include "Shader.h"

class Grid : public Mesh {
private:
    const float vertices[12] = {
        -50.f, 0.f, -50.f,
        -50.f, 0.f, 50.f,
        50.f, 0.f, -50.f,
        50.f, 0.f, 50.f
    };

    std::vector<GLuint> primitives;

    std::vector<float> vertexData;

    Shader::Uniform* worldMat;

protected:
    virtual void generateData() override;
    virtual void uploadData() override;
    virtual void renderInternal() override;

public:
    Vector4f color;

    Grid(Shader* shd);
};

#endif // GRID_H_INCLUDED

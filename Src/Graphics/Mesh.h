#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/glew.h>

class Shader;

class Mesh {
private:
    Shader* shader;

    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint primitiveBuffer = 0;

protected:
    GLsizei primitiveCount = 0;

    bool needsGen;
    bool needsUpload;
    virtual void generateData()=0;
    virtual void uploadData()=0;

public:
    Mesh(Shader* shd);
    ~Mesh();

    void render();
};

#endif // MESH_H_INCLUDED

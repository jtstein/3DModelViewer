
#ifndef GRID_H
#define GRID_H

#include "Shader.h"
#include "Vertex.h"
#include "Color3.h"

struct GridVertex {
    Vector3f position;
    Color3f color;
};

class Grid {
public:
    Grid();
    virtual ~Grid();

    void loadShader(const std::string& vertexShader, const std::string& fragmentShader);
    void construct(const Color3f& color, const Color3f& majorColor, float min, float max, float inc, unsigned int major);

    void beginRender();
    void endRender();

    std::shared_ptr<Shader>& getShader();

protected:
    void constructOnGPU();

protected:
    std::shared_ptr<Shader> shader;
    std::vector<GridVertex> vertices;
    unsigned int vboId;
};

#endif

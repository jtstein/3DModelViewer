#include "Grid.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const static unsigned int POSITION_LOC = 0;
const static unsigned int COLOR_LOC = 1;

Grid::Grid() {
    this->vboId = 0;
    this->shader = nullptr;
}

Grid::~Grid() {}

void Grid::loadShader(const std::string& vertexShader, const std::string& fragmentShader) {
    this->shader = std::make_shared<Shader>();
    this->shader->load(vertexShader, fragmentShader);

    this->shader->compile();
    glBindAttribLocation(this->shader->getProgramID(), POSITION_LOC, "position");
    glBindAttribLocation(this->shader->getProgramID(), COLOR_LOC, "color");
    this->shader->link();
}

void Grid::construct(const Color3f& color, const Color3f& majorColor, float min, float max, float inc, unsigned int major) {
    GridVertex v;
    v.color = color;

    unsigned int mod = 0;
    for ( float i = min; i <= max; i += inc) {
        v.position = Vector3f(i, 0, max);
        this->vertices.push_back(v);

        v.position = Vector3f(i, 0, min);
        this->vertices.push_back(v);

        v.position = Vector3f(max, 0, i);
        this->vertices.push_back(v);

        v.position = Vector3f(min, 0, i);
        this->vertices.push_back(v);

        if ( mod == 0 ) v.color = majorColor;
        else v.color = color;

        mod++;

        if ( mod > major ) mod = 0;
    }

    this->constructOnGPU();
}

void Grid::constructOnGPU() {
    glGenBuffers(1, &this->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GridVertex), &this->vertices[0], GL_STATIC_DRAW);
}

void Grid::beginRender() {
    if ( this->shader != nullptr ) this->shader->enable();

    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

    glEnableVertexAttribArray(POSITION_LOC);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertex), BUFFER_OFFSET(3 * sizeof(float)));
}

void Grid::endRender() {
    glDrawArrays(GL_LINES, 0, this->vertices.size());

    if ( this->shader != nullptr ) this->shader->disable();
}

std::shared_ptr<Shader>& Grid::getShader() {
    return this->shader;
}

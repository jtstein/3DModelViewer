
#ifndef FACE_H
#define FACE_H

#include "Vertex.h"

enum TriangleFaceComponent { A, B, C, TRIANGLE_EDGE_COUNT };

struct TriangleFace {
    operator const unsigned int* const () const { return this->indices; }
    unsigned int indices[TRIANGLE_EDGE_COUNT];
};

#endif

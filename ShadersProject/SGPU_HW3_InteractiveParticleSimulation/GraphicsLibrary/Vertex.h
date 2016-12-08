
#ifndef VERTEX_H
#define VERTEX_H

#include <Vector3.h>
#include <unordered_map>
#include "Color3.h"

/*
 * Vertex Structure:
 * ------------------------------------------------------------------------------
 * | px | py | pz | nx | ny | nz | tw | tx | ty | tz | tu | tv | tw | r | g | b |
 * ------------------------------------------------------------------------------
 */
struct Vertex {
    float operator () (const Vertex& v) const {
        float ret = v.position.x() + v.position.y() + v.position.z();
        ret += v.normal.x() + v.normal.y() + v.normal.z();
        ret += v.tangent.x() + v.tangent.y() + v.tangent.z() + v.tangent.w();
        ret += v.textureCoord.x() + v.textureCoord.y();
        return ret;
    }

    bool operator () (const Vertex& u, const Vertex& v) const {
        if ( u.position == v.position &&
             u.normal == v.normal &&
             u.tangent == v.tangent &&
             u.textureCoord == v.textureCoord ) return true;
        return false;
    }

    Vector3f position;
    Vector3f normal;
    Vector4f tangent;
    Vector3f textureCoord;
    Color3f color;
};

typedef std::unordered_map<Vertex, unsigned int, Vertex, Vertex> VertexSet;

#endif

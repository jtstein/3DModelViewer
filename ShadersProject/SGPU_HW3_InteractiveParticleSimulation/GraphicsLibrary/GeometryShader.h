
#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H

#include <string>
#include "Shader.h"

class GeometryShader : public Shader {
public:
    GeometryShader();
    virtual ~GeometryShader();

    virtual bool load(const std::string& vertexFilename, const std::string& geometryFilename, const std::string& fragmentFilename);
    virtual bool compile();
    virtual bool link();

protected:
    unsigned int geometryId;
    std::string geomFilename;
    std::string geomSource;
};

#endif

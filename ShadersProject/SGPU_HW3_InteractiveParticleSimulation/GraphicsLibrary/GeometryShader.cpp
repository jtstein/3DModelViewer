#include "GeometryShader.h"

GeometryShader::GeometryShader() {
    this->geometryId = 0;
}

GeometryShader::~GeometryShader() {
    glDeleteShader(this->geometryId);
}

bool GeometryShader::load(const std::string& vertexFilename, const std::string& geometryFilename, const std::string& fragmentFilename) {
    if ( !this->loadFile(vertexFilename, this->vertSource) ) return false;
    if ( !this->loadFile(fragmentFilename, this->fragSource) ) return false;
    if ( !this->loadFile(geometryFilename, this->geomSource) ) return false;

    this->vertFilename = vertexFilename;
    this->fragFilename = fragmentFilename;
    this->geomFilename = geometryFilename;

    this->vertexId = glCreateShader(GL_VERTEX_SHADER);
    this->fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    this->geometryId = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vsource_cstr = this->vertSource.c_str();
    const char* fsource_cstr = this->fragSource.c_str();
    const char* gsource_cstr = this->geomSource.c_str();

    glShaderSource(this->vertexId, 1, &vsource_cstr, 0);
    glShaderSource(this->fragmentId, 1, &fsource_cstr, 0);
    glShaderSource(this->geometryId, 1, &gsource_cstr, 0);
    return true;
}

bool GeometryShader::compile() {
    glCompileShader(this->vertexId);
    if ( !this->compileStatus(this->vertexId, this->vertFilename) ) return false;

    glCompileShader(this->fragmentId);
    if ( !this->compileStatus(this->fragmentId, this->fragFilename) ) return false;

    glCompileShader(this->geometryId);
    if ( !this->compileStatus(this->geometryId, this->geomFilename) ) return false;
    return true;
}

bool GeometryShader::link() {
    this->programId = glCreateProgram();
    glAttachShader(this->programId, this->vertexId);
    glAttachShader(this->programId, this->geometryId);
    glAttachShader(this->programId, this->fragmentId);
    glLinkProgram(this->programId);
    
    if ( !this->linkStatus(this->programId) ) return false;
    return true;
}

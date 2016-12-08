#include "EnvironmentMap.h"
#include <sstream>
#include <iostream>
#include "PNG.h"

#ifndef GL_EXT_texture_cube_map
#define GL_NORMAL_MAP_EXT                   0x8511
#define GL_REFLECTION_MAP_EXT               0x8512
#define GL_TEXTURE_CUBE_MAP_EXT             0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT     0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT  0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT  0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT  0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT  0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT  0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT  0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT       0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT    0x851C
#endif

static GLenum targets[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};

char* cube_face_names[6] =
{
    "_left", 
    "_right", 
    "_top", 
    "_bottom", 
    "_back", 
    "_front", 
};

EnvironmentMap::EnvironmentMap() {
    this->id = 0;
    this->images.resize(6);
}

EnvironmentMap::~EnvironmentMap() {}

bool EnvironmentMap::load(const std::string& basename, std::string ext) {
    if ( basename.length() == 0 ) return false;
    std::stringstream stream;

    glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_CUBE_MAP_EXT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

    for ( unsigned int i = 0; i < 6; i++ ) {
        stream << basename << cube_face_names[i] << "." << ext;

        this->load(targets[i], i, stream.str());
        stream.str("");
    }

    return true;
}

bool EnvironmentMap::load(GLenum target, unsigned int index, const std::string& filename) {
    if ( filename.length() == 0 ) return false;

    unsigned int width, height;
    unsigned int error = lodepng::decode(this->images[index], width, height, filename);

    if ( error ) {
        std::cerr << "[EnvironmentMap:load] Error: Could not load PNG image: " << filename << std::endl;
        return false;
    }

    glGenTextures(1, &this->id);
    glBindTexture(target, this->id);
    glTexImage2D(target, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &this->images[index][0]);
    return true;
}

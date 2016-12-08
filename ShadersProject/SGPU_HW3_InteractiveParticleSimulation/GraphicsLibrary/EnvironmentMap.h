
#ifndef ENVIRONMENT_MAP_H
#define ENVIRONMENT_MAP_H

#include <string>
#include <vector>
#include <gl/freeglut.h>

class EnvironmentMap {
public:
    EnvironmentMap();
    ~EnvironmentMap();

    bool load(const std::string& basename, std::string ext = "png");

protected:
    bool load(GLenum target, unsigned int index, const std::string& filename);

protected:
    unsigned int id;
    std::vector<std::vector<unsigned char> > images;
};

#endif

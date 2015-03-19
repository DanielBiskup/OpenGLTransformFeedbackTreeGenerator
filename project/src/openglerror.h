#ifndef OPENGLERROR_H
#define OPENGLERROR_H

#include <string>
#include <iostream>
#include <GL/glew.h>

std::string errorDescription(GLenum error);
std::string errorName(GLenum error);
void printError(const char *file, int line);

#define glErr() printError(__FILE__,__LINE__)


#endif // OPENGLERROR_H

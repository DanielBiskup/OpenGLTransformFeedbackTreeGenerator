// NICHT MEIN EIGENTUM!
// DIESER QUELLTEXT WURDE GEKLAUT VON:
// http://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/

#ifndef OPENGLERRORCALLBACK_H
#define OPENGLERRORCALLBACK_H

#include <GL/glew.h>

void openglCallbackFunction(unsigned int source,
									 unsigned int type,
									 unsigned int id,
									 unsigned int severity,
									 int length,
									 const char* message,
									 const void* userParam);

void registerDebugMessageCallback();


#endif // OPENGLERRORCALLBACK_H

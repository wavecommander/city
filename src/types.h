#if defined(_WIN64) || defined(_WIN32)
typedef unsigned int uint;
#endif

#include "../wolf/W_Types.h"

struct Vertex
{
    GLfloat x,y,z;
    GLfloat u,v;
};

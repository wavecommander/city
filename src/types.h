#ifndef S_TYPES_H
#define S_TYPES_H

#include "../wolf/W_Types.h"

#if defined(_WIN64) || defined(_WIN32)
typedef unsigned int uint;
#endif

struct Vertex
{
    GLfloat x,y,z;
    GLfloat u,v;
};

enum class CellType
{
    UNSET = 0,
    ROAD_X,
    ROAD_Z,
    INTER,
    GRASS,
    CONCRETE,
    ASPHALT,
    N_BASIC
};

#endif // S_TYPES_H

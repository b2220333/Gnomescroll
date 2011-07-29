#include <stdio.h>
#include <stdlib.h>

// cube properties

struct cubeProperties {
    int active;
    int occludes;
    int solid;
    int gravity;
    int transparent;
};

struct cubeProperties* _get_cube_list();
struct cubeProperties* _get_cube(int id);

//buffer

struct Vertex* _get_quad_cache();

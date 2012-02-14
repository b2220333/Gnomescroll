#include "options.hpp"

#include <stdlib.h>

namespace Options
{

#define OPT_BOOL(NAME, DEFAULT)\
bool NAME = DEFAULT;\
void set_##NAME(bool val)\
{\
    NAME = val;\
}

#define OPT_FLOAT(NAME, DEFAULT)\
float NAME = DEFAULT;\
void set_##NAME(float val)\
{\
    NAME = val;\
}

#define OPT_INT(NAME, DEFAULT)\
int NAME = DEFAULT;\
void set_##NAME(int val)\
{\
    NAME = val;\
}

#define OPT_STRING(NAME, DEFAULT)\
static char NAME##_default[] = DEFAULT;\
char* NAME = NAME##_default;\
void set_##NAME(char* val)\
{\
    unsigned int len = strlen(val);\
    if (NAME == NULL || NAME == NAME##_default)\
    {\
        NAME = (char*)malloc(sizeof(char) * (len+1));\
    }\
    else\
    {\
        if (len != strlen(NAME))\
        {\
            NAME = (char*)realloc(NAME, sizeof(char)*(len+1));\
        }\
    }\
    strcpy(NAME, val);\
}

/* User */
OPT_STRING(name, "default-name")

/* Network */
OPT_STRING(server, "0.0.0.0")
OPT_INT(port, 0)

/* Game preferences */
OPT_BOOL(auto_assign_team, true)

/* Window / Camera */
OPT_INT(width, 1024)
OPT_INT(height, 800)
OPT_BOOL(fullscreen, false)
OPT_FLOAT(fov, 85.0f)

/* Controls */
OPT_FLOAT(sensitivity, 70.0f)
OPT_FLOAT(camera_speed, 0.6)
OPT_BOOL(invert_mouse, false)

/* HUD / Info */
OPT_BOOL(hud, true)
OPT_BOOL(diagnostic_hud, true)
OPT_BOOL(fps, true)
OPT_BOOL(ping, true)
OPT_INT(ping_update_interval, 500)

/* Font */
OPT_STRING(font, "inc_18")
OPT_INT(font_size, 18)

/* Sound */
OPT_BOOL(sound, false)
OPT_INT(sfx, 100)
OPT_INT(music, 100)

}

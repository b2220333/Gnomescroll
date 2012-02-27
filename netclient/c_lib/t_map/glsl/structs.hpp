#pragma once

namespace t_map
{
    struct ColorElement
    {
        union
        {
            struct
            {
                unsigned char r,g,b,a;
            };
            unsigned int color;
        };
    };

    struct AOElement
    {
        union
        {
            struct
            {
                unsigned char ao[4]; //32
            };
            unsigned int AO;
        };
    };

    struct NormalElement
    {
        union
        {
            char normal[4]; //16
            unsigned int n;
        };
    };
    
    /*
        Replace x,y,z and tx,ty,tz by char
        to reduce size from x 36 to 16 bytes per vertex for normal blocks
    */
    struct Vertex {
        struct
        {
            unsigned char x,y,z; //3
            unsigned char tx,ty,tz; //6

            unsigned char r,g,b; //9

            char normal[3]; //12

            /*
                Replace normal with uniform lookup
                this frees up 2 bytes (one more byte for texture and one for alpha)
            */

        };

        union
        {
            unsigned char ao[4]; 
            unsigned int AO;
        };
    };
    
}

/*
    struct Vertex {
        float x,y,z; //12
        float tx,ty,tz; //24

        union
        {
            struct
            {
                unsigned char r,g,b,a;
            };
            unsigned int color;
        };

        union
        {
            char normal[4]; //16
            unsigned int n;
        };

        union
        {
            unsigned char ao[4]; //32
            unsigned int AO;
        };
    };
*/
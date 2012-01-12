#include "hitscan.hpp"

#include <c_lib/state/client_state.hpp>
#include <stdlib.h>

#include <c_lib/camera/camera.hpp>

#include <compat_gl.h>
#include <c_lib/SDL/texture_loader.h>
#include <c_lib/physics/vector.hpp>

namespace Animations
{

static int hitscan_texture_id;

void init_hitscan()
{
    int i;
    i = create_texture_from_file( (char*) "./media/texture/hitscan/hitscan_01.png", &hitscan_texture_id);
    if (i) { printf("init_hitscan failed with code %d\n", i); }
}

void create_hitscan(float x,float y,float z, float vx, float vy, float vz)
{
    HitscanEffect* he = ClientState::hitscan_effect_list.create();

    if(he == NULL) return;

    he->x=x; he->y=y; he->z=z;
    he->vx=vx; he->vy=vy; he->vz=vz;
   
    //printf("Hitscan created: id= %i \n", he->id);
}

void HitscanEffect::tick()
{
    x += vx /30.0;
    y += vy /30.0;
    z += vz /30.0;    

    //check for collision with terrain/players
    //play animations for terrain/player collision
}

void HitscanEffect::draw(float delta)
{
    const float width = 0.50;
    const float height = 1.0/4.0;   //length per velocity


    float _x = x + vx*delta;
    float _y = y + vy*delta;
    float _z = z + vz*delta;  

    _x = x;
    _y = y;
    _z = z;

    struct Vector c = Vector_init(current_camera->x, current_camera->y, current_camera->z);

    struct Vector r = Vector_init(vx,vy,vz);
    normalize_vector( &r );
    //vector_scalar2(&r)

    //printf("r length= %f \n", vector_length( &r ) );

    struct Vector x1 = Vector_init(_x,_y,_z);
    struct Vector l1 = sub_vec(&x1, &c);

    struct Vector u1 = vector_cross( l1, r);
    normalize_vector( &u1 );
    //printf("u1 length= %f \n", vector_length( &u1 ) );

    // float norm = sqrt(vx*vx+vy*vy+vz*vz);
    // struct Vector x2 = Vector_init(x- height*vx/norm, y - height*vy/norm, z - height*vz/norm);
    struct Vector x2 = Vector_init(_x- height*vx, _y - height*vy, _z - height*vz);

    struct Vector l2 = sub_vec(&x2, &c);

    struct Vector u2 = vector_cross( l2, r);
    normalize_vector( &u2 );
    
    //printf("u2 length= %f \n", vector_length( &u2 ) );

    vector_scalar1(&u1, width);
    vector_scalar1(&u2, width);  

/*
    Vector top_left = Vector_init(x1.x + width*u1.x, x1.y + width*u1.y, x1.z + width*u1.z);
    Vector top_right = Vector_init(x1.x - width*u1.x, x1.y - width*u1.y, x1.z - width*u1.z);
    Vector bottom_right = Vector_init(x2.x - width*u2.x, x2.y - width*u2.y, x2.z - width*u2.z);
    Vector bottom_left = Vector_init(x2.x + width*u2.x, x2.y + width*u2.y, x2.z + width*u2.z);
*/
    const float tx_min = 0.0;
    const float tx_max = 1.0;
    const float ty_min = 0.0;
    const float ty_max = 1.0;


    glTexCoord2f(tx_max,ty_max );
    glVertex3f( x2.x + u2.x, x2.y + u2.y, x2.z + u2.z );  // Bottom left
    
    glTexCoord2f(tx_min,ty_max );
    glVertex3f( x1.x + u1.x, x1.y + u1.y, x1.z + u1.z);  // Top left
    
    glTexCoord2f(tx_min,ty_min );
    glVertex3f( x1.x - u1.x, x1.y - u1.y, x1.z - u1.z );  // Top right

    glTexCoord2f(tx_max,ty_min);
    glVertex3f( x2.x - u2.x, x2.y - u2.y, x2.z - u2.z );  // Bottom right

}


/* list */

void HitscanEffect_list::draw()
{
    //printf("draw \n");

    int last_tick = _LAST_TICK();
    int _t = _GET_MS_TIME();
    //printf("ms since last update= %i \n", _t - last_tick);
    float delta = ((float)(_t - last_tick)) / 33.3f;
    if(delta > 1.0f)
    {
        delta = 1.0f;
    }
    delta /= 30.0f;
    //printf("delta= %f \n", delta);

    glColor3ub(255,255,255);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glBindTexture( GL_TEXTURE_2D, hitscan_texture_id );
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    glBegin( GL_QUADS );

    //int count= 0;
    int i;
    for(i=0; i<n_max; i++) {
        if (a[i] == NULL) continue;
        printf("Drawing %d\n", i);
        a[i]->draw(delta);
        //count++;
    }
    //printf("count= %i \n", count);

    glEnd();
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glDisable (GL_DEPTH_TEST);
    glDisable(GL_BLEND);

}

void HitscanEffect_list::tick()
{
    //printf("tick \n");

    const int debug = 1;

    if(debug)
    {
        static int frame = 0;
        frame++;

        if(frame % 8 == 0 )
        {
        const float vm = 16.0;
        float vx = vm*(float)rand()/(float)RAND_MAX;
        float vy = 160.0; //+vm*(float)rand()/(float)RAND_MAX;
        float vz = vm*(float)rand()/(float)RAND_MAX;
        ClientState::hitscan_effect_list.create(32.0, 32.0, 64.0, vx, vy, vz);
        }


        //ClientState::hitscan_effect_list.create(0.0, 0.0, 0.0, -0.0, 0.0, 0.0);
    }

    create_hitscan(0.0, 0.0, 0.0, 0.0, 0.0, 160.0);

    //int count= 0;
    int i;
    for(i=0; i<n_max; i++) {
        if (a[i] == NULL) continue;
        a[i]->tick();
        a[i]->ttl--;
        if(a[i]->ttl == 0) destroy(a[i]->id);
        //count++;
    }
    //printf("count= %i \n", count);
}

}


#include "_interface.hpp"

#include <t_mech/net/CtoS.hpp>
#include <t_mech/net/StoC.hpp>

#if DC_CLIENT
    #include <camera/camera.hpp>
    #include <physics/ray_trace/hitscan.hpp>
    #include <t_mech/mech_draw.hpp>
#endif

#include <t_mech/mech_state.hpp>


namespace t_mech
{

class MECH_LIST* mech_list;

void init_packets()
{
    mech_create_StoC::register_client_packet();
    mech_delete_StoC::register_client_packet();
}



#if DC_CLIENT
class MechListRenderer* mech_list_renderer = NULL;
#endif

void init() 
{
    mech_list = new MECH_LIST;
#if DC_CLIENT
    mech_list_renderer = new MechListRenderer;
#endif
    init_properties();
}

void teardown() 
{
    tear_down_properties();
    delete mech_list;
    
#if DC_CLIENT
    delete mech_list_renderer;
#endif

}

#if DC_CLIENT

void prep()
{
    mech_list_renderer->prep_vbo();
}

void draw()
{
    mech_list_renderer->draw();
}

void draw_transparent()
{
    mech_list_renderer->draw_transparent();
}
#endif

/*
    MECH_CRYSTAL,
    MECH_CROP,
    MECH_WIRE,
    MECH_SWITCH
*/

/*
    MECH_CRYSTAL,
    MECH_CROP,
    MECH_WIRE,
    MECH_SWITCH
*/
    

//pack mech data into packet
static void pack_mech(struct MECH &m, class mech_create_StoC &p)
{
#if DC_SERVER
    p.id = m.id;
    p.mech_type = m.mech_type;
    p.subtype = m.subtype;
    p.x = m.x;
    p.y = m.y;
    p.z = m.z;

    switch ( m.mech_type )
    {
    case MECH_CRYSTAL:
        //do something
        break;
    default:
        printf("pack_mech error: unhandled mech type\n");
    }
#endif
}


//handles unpacking
static void unpack_mech(struct MECH &m, class mech_create_StoC &p)
{
#if DC_CLIENT
    m.id = p.id;
    m.mech_type = p.mech_type;
    m.subtype = p.subtype;
    m.x = p.x;
    m.y = p.y;
    m.z = p.z;

    switch ( p.mech_type )
    {
    case MECH_CRYSTAL:
        //do something

        m.size = 0.80;  //radius
        m.rotation = 0.25*(rand()%4) + 0.25f*randf()/3;
        m.offset = rand()%255;
        //m.subtype = rand()%6;

        m.offset_x = (randf()-0.5f)* (1.0f-m.size);
        m.offset_y = (randf()-0.5f)* (1.0f-m.size);

        break;
    default:
        printf("pack_mech error: unhandled mech type\n");
    } 
#endif
}

void handle_block_removal(int x, int y, int z)
{


}

void create_crystal(int x, int y, int z)
{
#if DC_SERVER

    struct MECH m;
    m.mech_type = MECH_CRYSTAL;
    m.x = x;
    m.y = y;
    m.z = z;
    m.subtype = rand()%9;

    mech_list->server_add_mech(m);
#endif
}

bool can_place_crystal(int x, int y, int z, int side)
{
    if( isSolid(x,y,z) == true)
        return false;

    if(side != 0)
        return false;

    //check if there is another one on this square
    return true;
}

void place_vine(int x, int y, int z, int side)
{


}


void tick(int x, int y, int z)
{
#ifdef DC_CLIENT
    //printf("create crystal: %d %d %d \n", x,y,z);
    //create_crystal(x,y,z);
#endif
}

#if DC_CLIENT

bool ray_cast_mech_render_type_0(const class MECH &m, float x, float y, float z, float vx, float vy, float vz, float* distance)
{
    const float size2 = m.size/2.0f;
    float wx = (float) (m.x) + 0.5f + m.offset_x;
    float wy = (float) (m.y) + 0.5f + m.offset_y;
    float wz = (float) m.z + size2;

    wx = quadrant_translate_f(current_camera_position.x, wx);
    wy = quadrant_translate_f(current_camera_position.y, wy);

    //float dx = sin(m.rotation * PI);
    //float dy = cos(m.rotation * PI);

    //translate into origin of plane
    x -= wx;
    y -= wy;
    z -= wz;

    return true;
/*
    float _x 
    m.size
    m.rotation
    m.offset_x
    m.offset_y
*/
}

bool ray_cast_mech(float x, float y, float z, float vx, float vy, float vz, float* _distance)
{
    //int nearest_mech = -1;
    //float distance = 1000.0;

    const int mlm = mech_list->mlm;
    const struct MECH* mla = mech_list->mla;

    for(int i=0; i<mlm; i++)
    {
        if( mla[i].id == -1) continue;

        float d;
        bool ret;

        switch ( mla[i].render_type )
        {
        case MECH_RENDER_TYPE_0: //MECH_CRYSTAL:
            //do something
            ret = ray_cast_mech_render_type_0(mla[i], x,y,z, vx,vy,vz, &d);
            if(ret == true)
            {
                printf("mech raycast hit: %i \n", i);
                return true;
            }
            break;
        default:
            printf("pack_mech error: unhandled mech type\n");
        }
    }

    return false;
}
#endif

}

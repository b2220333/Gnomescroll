#pragma once

#include <t_mech/_interface.hpp>
#include <t_mech/mech_state.hpp>
#include <t_mech/_interface.hpp>

#include <t_mech/config/_interface.hpp>
#include <t_mech/mesh_loader.hpp>

#if DC_SERVER
dont_include_this_file_in_server
#endif

#include <physics/vec3.hpp>
#include <animations/common.hpp>


using Animations::VertexElementListTexture;

namespace t_mech
{



class MechLightEffect
{
    public:

    unsigned int texture;
    class Shader shader;
    unsigned int shader_TexCoord;

    VertexElementListTexture* vlist;

    void init()
    {
        vlist = NULL;
        vlist = new VertexElementListTexture;

        init_texture();
        init_shader();
    }


    MechLightEffect()
    {
    }

    ~MechLightEffect()
    {
        delete vlist;
    }

    void init_texture()
    {
        SDL_Surface* s = create_surface_from_file(MEDIA_PATH "sprites/mech/light_00.png");
        IF_ASSERT(s == NULL)
            return;

        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        GLenum internalFormat = 4; //GL_RGBA;
        GLenum format = get_texture_format(s);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);
        glDisable(GL_TEXTURE_2D);

        //SDL_FreeSurface(s);
    }

    void init_shader()
    {
        shader.set_debug(false);

        shader.load_shader("mech light source shader",
            MEDIA_PATH "shaders/effect/light_source.vsh",
            MEDIA_PATH "shaders/effect/light_source.fsh");

        shader_TexCoord = shader.get_attribute("InTexCoord");
    }


    inline void push_light(const struct Mech &m)
    {

        float wx = (float) (m.x) + 0.5f + m.offset_x;
        float wy = (float) (m.y) + 0.5f + m.offset_y;
        float wz = (float) m.z;
        //fulstrum test

        const float cx = current_camera_position.x;
        const float cy = current_camera_position.y;

        wx = quadrant_translate_f(cx, wx);
        wy = quadrant_translate_f(cy, wy);

        if (!sphere_fulstrum_test(wx, wy, wz, 0.6f))
            return;

        const int tex_id = rand()%16;
        //GS_ASSERT(mech_attributes[m.type].type != -1);

        const float txmargin = 0.0f;
        float tx_min, ty_min, tx_max, ty_max;

        int ti = tex_id % 16;
        int tj = tex_id / 16;

        const float h = 0.0625f;

        tx_min = ti*h + txmargin;
        ty_min = tj*h + txmargin;
        tx_max = ti*h + h - txmargin;
        ty_max = tj*h + h - txmargin;


    /*
        float x = quadrant_translate_f(current_camera_position.x, this->x);
        float y = quadrant_translate_f(current_camera_position.y, this->y);
        float z = this->z;


        v1 = vec3_init(x, y, z2+z);
        v2 = vec3_init(x+tw*sinf(f1*i), y+th*cosf(f1*i), z0+z);
        v3 = vec3_init(x+tw*sinf(f1*(i+1)), y+th*cosf(f1*(i+1)), z0+z);

        vlist->push_vertex(v1, 0.5f, 0.5f);
        vlist->push_vertex(v2, sinf(f1*i)/2 + 0.5f,  cosf(f1*i)/2 + 0.5f);
        vlist->push_vertex(v3, sinf(f1*(i+1))/2 + 0.5f,  cosf(f1*(i+1))/2 + 0.5f);
    */
    }


    void prep()
    {
        //prep_light0();

        const MechType mech_type = t_mech::get_mech_type_dat("light_crystal");

        const int mlm = mech_list->mlm;
        const struct Mech* mla = mech_list->mla;


        for (int i=0; i<mlm; i++)
        {
            if (mla[i].id == -1) continue;
            if(mla[i].id == mech_type)
                push_light(mla[i]);
        }

        vlist->buffer();
    }

    void draw()
    {

        if (vlist->vertex_number == 0) return;

        GS_ASSERT(vlist->VBO != 0);
        if (vlist->VBO == 0) return;

        glColor3ub(255,255,255);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindBuffer(GL_ARRAY_BUFFER, vlist->VBO);

        glUseProgramObjectARB(shader.shader);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableVertexAttribArray(shader_TexCoord);

        int offset = 0;
        glVertexPointer(3, GL_FLOAT, vlist->stride, (GLvoid*)offset);
        offset += 3 * sizeof(GL_FLOAT);
        glVertexAttribPointer(shader_TexCoord, 2, GL_FLOAT, GL_FALSE, vlist->stride, (GLvoid*)offset);

        glDrawArrays(GL_TRIANGLES,0, vlist->vertex_number);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableVertexAttribArray(shader_TexCoord);
        glUseProgramObjectARB(0);


    }
};



}   // t_mech
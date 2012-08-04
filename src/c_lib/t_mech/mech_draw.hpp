#pragma once 

namespace t_mech 
{

//class MechListVertexList 
//class MechListShader
//class MechListRenderer

/*
    Use this as example for other shaders
*/

class MechListVertexList 
{
    public:
    // visibility will default to private unless you specify it
    struct Vertex
    {
        float x,y,z;
        float tx,ty;
        float btx,bty;  //texture cordinates for bilinear interpolation
        unsigned char color[4];
        unsigned char brightness[4];
    };

    static const int stride = sizeof(Vertex);

    struct Vertex* va;  //vertex array
    int vi; //vertex index
    int vm; //vertex max

    unsigned int VBO; //for drawing 

    struct Vertex v; //set this and then push vertex

    MechListVertexList()
    {
        vm = 32;
        vi = 0;
        va = (struct Vertex*) malloc(vm*sizeof(struct Vertex));
        VBO = 0;
    }

    ~MechListVertexList()
    {
        free(va);
    }
    

    void vertex3f(float x, float y, float z)
    {
        v.x = x;
        v.y = y;
        v.z = z;
    }

    void tex2f(float tx, float ty)
    {
        v.tx = tx;
        v.ty = ty;
    }

    void push_vertex()
    {
        va[vi] = v;
        vi++;
        if(vi == vm)
        {
            vm *= 2;
            va = (struct Vertex*) realloc(va, vm*sizeof(struct Vertex));
        }
    }

    void reset()
    {
        vi = 0;
    }

    void buffer()
    {
        if(VBO == 0) glGenBuffers(1, &VBO);

        if(vi != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vi*stride, NULL, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, vi*stride, va, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } 
        else
        {
            if(vi > 0) 
            {
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }
    }

};



class MechListShader
{
    public:

    SDL_Surface* s;

    unsigned int texture1;
    class SHADER* shader;

    //uniforms
    unsigned int CameraPosition;
    //attributes
    unsigned int TexCoord;
    unsigned int Brightness;

    MechListShader()
    : s(NULL), shader(NULL)
    {
        init_texture();
        init_shader();
    }

    ~MechListShader()
    {
        if (s != NULL) SDL_FreeSurface(s);
        if (shader != NULL) delete shader;
    }

    void init_shader()
    {
        shader = new SHADER;
        shader->set_debug(true);

        shader->load_shader( "mech_list_shader",
            "./media/shaders/effect/mech_list.vsh",
            "./media/shaders/effect/mech_list.fsh" );

        CameraPosition =    shader->get_uniform("CameraPosition");

        TexCoord    =       shader->get_attribute("InTexCoord");
        Brightness  =       shader->get_attribute("InBrightness");
    }

    void init_texture()
    {
        s = create_surface_from_file("./media/sprites/mech_00.png");

        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        GLenum texture_format;
        if (s->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, texture_format, GL_UNSIGNED_BYTE, s->pixels); //2nd parameter is level
        
        glDisable(GL_TEXTURE_2D);
    }

};


class MechListRenderer
{
    public:

    class MechListShader shader;
    class MechListVertexList vertex_list;

    void draw()
    {

        //printf("draw: %i \n", vertex_list.vi);

        if(vertex_list.vi == 0) return;
        if(shader.shader->shader_valid == false) return;

        //printf("draw \n");

        glColor3ub(255,255,255);

        GL_ASSERT(GL_DEPTH_TEST, true);
        GL_ASSERT(GL_DEPTH_WRITEMASK, false);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_list.VBO);

        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, shader.texture1 );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        shader.shader->enable_attributes();

        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, vertex_list.stride, (GLvoid*)0);
        glVertexAttribPointer(shader.TexCoord, 2, GL_FLOAT, GL_FALSE, vertex_list.stride, (GLvoid*)12);

        //glUniform4f(InTranslation, (GLfloat*) p.f );
        //glUniform3fv(InTranslation, 1, (GLfloat*) p.f );

        glDrawArrays(GL_QUADS, 0, vertex_list.vi);

        glDisableClientState(GL_VERTEX_ARRAY);

        shader.shader->disable_attributes();

        glBindTexture( GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }


    struct MECH_LIST_RENDER
    {
        short x,y,z;
        unsigned char face; //block side and edge
        unsigned char tex;
        unsigned char r,g,b;    //color
    };

    struct MECH_LIST_RENDER* mlra; // MECH_LIST_RENDER_list array
    int mlri;   //index
    int mlrm;   //max

    class MECH_LIST* ml; //control node list

    MechListRenderer(class MECH_LIST* _ml)
    {
        this->mlri = 0;
        this->mlrm = 32;
        this->ml = _ml;
        this->mlra = (struct MECH_LIST_RENDER*) malloc(this->mlrm*sizeof(struct MECH_LIST_RENDER));
    }

    ~MechListRenderer()
    {
        free(mlra);
    }

    void prep_vbo();

};


void MechListRenderer::prep_vbo()
{
    static const float vin[72] = 
    {
        1,1,1, 0,1,1, 0,0,1, 1,0,1, //top
        0,1,0, 1,1,0, 1,0,0, 0,0,0, //bottom
        1,0,1, 1,0,0, 1,1,0, 1,1,1, //north
        0,1,1, 0,1,0, 0,0,0, 0,0,1, //south
        1,1,1, 1,1,0, 0,1,0, 0,1,1, //west
        0,0,1, 0,0,0, 1,0,0, 1,0,1  //east
    };


    vertex_list.reset();

    for(int i=0; i<mlri; i++)
    {

        float x = (float) mlra[i].x;
        float y = (float) mlra[i].y;
        float z = (float) mlra[i].z;
        int face = mlra[i].face;

        int tex_id = mlra[i].tex;

        const float txmargin = 0.0f;
        float tx_min, ty_min, tx_max, ty_max;

        int ti = tex_id % 16;
        int tj = tex_id / 16;

        const float h = 0.0625f;

        tx_min = ti*h + txmargin;
        ty_min = tj*h + txmargin;
        tx_max = ti*h + h - txmargin;
        ty_max = tj*h + h - txmargin;

        int s = face;

        vertex_list.vertex3f(x+vin[12*s +3*0 +0], y+vin[12*s+ 3*0 +1], z+vin[12*s +3*0 +2]);
        vertex_list.tex2f(tx_min,ty_min);
        vertex_list.push_vertex();

        vertex_list.vertex3f(x+vin[12*s +3*1 +0], y+vin[12*s+ 3*1 +1], z+vin[12*s +3*1 +2]);
        vertex_list.tex2f(tx_min,ty_max);
        vertex_list.push_vertex();

        vertex_list.vertex3f(x+vin[12*s +3*2 +0], y+vin[12*s+ 3*2 +1], z+vin[12*s +3*2 +2]);
        vertex_list.tex2f(tx_max,ty_max );
        vertex_list.push_vertex();

        vertex_list.vertex3f(x+vin[12*s +3*3 +0], y+vin[12*s+ 3*3 +1], z+vin[12*s +3*3 +2]);
        vertex_list.tex2f(tx_max,ty_min );
        vertex_list.push_vertex();
    }

    vertex_list.buffer();

}

/*
void MechListRenderer::MECH_LIST_RENDER_update()
{
    static int counter = 0; //refresh, deals with loading before terrain map
    counter++;

    //if(ml->needs_update == false && mlri != 0 && counter%30!=0) return;
    if(ml->needs_update == false && mlri != 0 ) return;
    ml->needs_update = false;
    mlri = 0; //reset index
}
*/

/*
void mech_list_render_init(class MECH_LIST* _ml)
{
    GS_ASSERT(MECH_LIST_RENDERer == NULL);
    MECH_LIST_RENDERer = new MechListRenderer(_ml);
}

void mech_list_render_teardown()
{
    if (MECH_LIST_RENDERer != NULL) delete MECH_LIST_RENDERer;
}

void mech_list_render_prep()
{
    MECH_LIST_RENDERer->update();
    MECH_LIST_RENDERer->prep_vbo();
}

void mech_list_render_draw()
{
    MECH_LIST_RENDERer->draw();
}
*/

}
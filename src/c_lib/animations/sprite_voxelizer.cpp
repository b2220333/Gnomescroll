#include "sprite_voxelizer.hpp"

#include <animations/common.hpp>
#include <voxel/constants.hpp>
#include <common/color.hpp>

namespace Animations
{

static class Shader sprite_voxelizer_shader;

static struct
{
    // attributes
    GLint normal;
    GLint color;
    GLint ao;

    // uniforms
    GLint camera_pos;
    GLint matrix;
} sprite_voxelizer_shader_vars;

const size_t SPRITE_VOXELIZER_MAX = MAX_ITEMS;
static VertexElementListColorByte* sprite_voxelizer_vlists[SPRITE_VOXELIZER_MAX] = {NULL}; 

static void push_sprite_vertex_cube(VertexElementListColorByte* vlist, float x, float y, const struct Color color)
{
    static struct Vec3 veb[8];     //vertex positions

    for (int i=0; i<8; i++)
    {
        veb[i].x = voxelized_sprite_config.scale*(v_set[3*i+0] + 0);
        veb[i].y = voxelized_sprite_config.scale*(v_set[3*i+1] + x);
        veb[i].z = voxelized_sprite_config.scale*(v_set[3*i+2] + y);
    }

    for (int i=0; i<6; i++)
    for (int j=0; j<4; j++)
        vlist->push_vertex(veb[q_set[4*i+j]], color, v_normal_b[i]);
}

static void generate_sprite_vertices(
    const class TextureSheetLoader::TextureSheetLoader* sheet_loader,
    int sprite_id)
{
    IF_ASSERT(sprite_id < 0 || sprite_id >= (int)SPRITE_VOXELIZER_MAX) return;
    const struct Color4* pixels = sheet_loader->get_sprite_pixels(sprite_id);
    IF_ASSERT(pixels == NULL) return;
    if (sprite_voxelizer_vlists[sprite_id] == NULL)
        sprite_voxelizer_vlists[sprite_id] = new VertexElementListColorByte;
    VertexElementListColorByte* vlist = sprite_voxelizer_vlists[sprite_id];
    vlist->clear();
    
    const size_t tile_size = sheet_loader->tile_size;
    const size_t n_pixels =  tile_size*tile_size;
    const unsigned char alpha_test = 0xFF/2;
    for (size_t i=0; i<n_pixels; i++)
    {   // build vertices from pixels
        if (pixels[i].a <= alpha_test) continue;
        float x = float((n_pixels-i-1) / tile_size);
        float y = float((n_pixels-i-1) % tile_size);
        
        push_sprite_vertex_cube(vlist, x, y, color_init_linear_scale(pixels[i]));

    }
    // check if the sprite wasn't completely invisible
    IF_ASSERT(vlist->vlist_index <= 0)
    {
        printf("Sprite id %d in use but is invisible after alpha testing\n", sprite_id);
        return;
    }

    vlist->buffer_static();
}

void load_sprite_voxelizer()
{   // generate vertex arrays from items in the spritesheet
    for (int i=0; i<MAX_ITEM_TYPES; i++)
    {
        class Item::ItemAttribute* attr = &Item::item_attributes[i];
        if (!attr->loaded || attr->particle_voxel) continue;
        generate_sprite_vertices(TextureSheetLoader::item_texture_sheet_loader, attr->sprite);
    }
}

void init_sprite_voxelizer()
{
    sprite_voxelizer_shader.set_debug(true);
    sprite_voxelizer_shader.load_shader("sprite voxelizer",
        "./media/shaders/animation/sprite_voxelizer.vsh",
        "./media/shaders/animation/sprite_voxelizer.fsh");

    sprite_voxelizer_shader_vars.normal = sprite_voxelizer_shader.get_attribute("InNormal");
    sprite_voxelizer_shader_vars.ao = sprite_voxelizer_shader.get_attribute("InAO");
    sprite_voxelizer_shader_vars.camera_pos = sprite_voxelizer_shader.get_uniform("InCameraPos");
    sprite_voxelizer_shader_vars.matrix = sprite_voxelizer_shader.get_uniform("InMatrix");
}

void teardown_sprite_voxelizer()
{
    for (size_t i=0; i<SPRITE_VOXELIZER_MAX; i++)
        if (sprite_voxelizer_vlists[i] != NULL)
            delete sprite_voxelizer_vlists[i];
}

void draw_voxelized_sprite(int sprite_id, const struct Mat4& rotation_matrix)
{
    IF_ASSERT(sprite_id < 0 || sprite_id >= (int)SPRITE_VOXELIZER_MAX) return;
    IF_ASSERT(sprite_voxelizer_vlists[sprite_id] == NULL) return;
    IF_ASSERT(sprite_voxelizer_shader.shader == 0) return;
    VertexElementListColorByte* vlist = sprite_voxelizer_vlists[sprite_id];
    IF_ASSERT(vlist->vertex_number == 0) return;

    glUseProgramObjectARB(sprite_voxelizer_shader.shader);

    glBindBuffer(GL_ARRAY_BUFFER, vlist->VBO);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnableVertexAttribArray(sprite_voxelizer_shader_vars.normal);
    glEnableVertexAttribArray(sprite_voxelizer_shader_vars.ao);

    struct Vec3 cpos = current_camera->get_position();
    glUniform3f(sprite_voxelizer_shader_vars.camera_pos, cpos.x, cpos.y, cpos.z);
    glUniformMatrix4fv(sprite_voxelizer_shader_vars.matrix, 1, GL_FALSE, rotation_matrix._f);

    size_t offset = 0;
    glVertexPointer(3, GL_FLOAT, vlist->stride, (GLvoid*)offset);
    offset += 3 * sizeof(GL_FLOAT);
    glColorPointer(3, GL_UNSIGNED_BYTE, vlist->stride, (GLvoid*)offset);
    offset += 3 * sizeof(GL_UNSIGNED_BYTE);
    
    glVertexAttribPointer(sprite_voxelizer_shader_vars.normal, 3, GL_BYTE, GL_FALSE, vlist->stride, (GLvoid*)offset);
    offset += 3 * sizeof(GL_BYTE);
    glVertexAttribPointer(sprite_voxelizer_shader_vars.ao, 4, GL_UNSIGNED_BYTE, GL_TRUE, vlist->stride, (GLvoid*)offset);
    offset += 4 * sizeof(GL_UNSIGNED_BYTE);
    
    glDrawArrays(GL_QUADS, 0, vlist->vertex_number);

    glDisableVertexAttribArray(sprite_voxelizer_shader_vars.normal);
    glDisableVertexAttribArray(sprite_voxelizer_shader_vars.ao);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glUseProgramObjectARB(0);

    CHECK_GL_ERROR();
}

bool draw_voxelized_sprite_gl_begin()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    GL_ASSERT(GL_TEXTURE_2D, false);
    GL_ASSERT(GL_BLEND, false);
    //GL_ASSERT(GL_DEPTH_TEST, true);
    return true;
}

void draw_voxelized_sprite_gl_end()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

}   // Animations

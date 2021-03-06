/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#include "textures.hpp"

namespace t_mob
{

class SpriteAnimationRepo* animations;

void init_animation_repo()
{
    GS_ASSERT(animations == NULL);
    animations = new class SpriteAnimationRepo;

    #if DC_CLIENT
    animations->load_texture("haltingstate");
    animations->load_texture("rdn");
    #endif
}

void teardown_animation_repo()
{
    delete animations;
}

void SpriteAnimation::add_frame(size_t x, size_t y)
{
    IF_ASSERT(this->count >= this->MAX_FRAMES) return;
    #if DC_CLIENT
    int tex = this->group->repo->texture_loader.blit(this->sheet, x, y);
    #else
    int tex = this->count;
    #endif
    GS_ASSERT(tex != NULL_SPRITE);
    this->frames[this->count++] = tex;
}

void SpriteAnimation::_add_frame_strip(size_t x, size_t y, size_t n, bool horizontal)
{
    for (size_t i=0; i<n; i++)
    {
        if (horizontal)
            this->add_frame(x + i, y);
        else
            this->add_frame(x, y + i);
    }
}

void SpriteAnimation::add_vertical_frame_strip(size_t x, size_t y, size_t n)
{
    this->_add_frame_strip(x, y, n, false);
}

void SpriteAnimation::add_horizontal_frame_strip(size_t x, size_t y, size_t n)
{
    this->_add_frame_strip(x, y, n, true);
}


}   // t_mob

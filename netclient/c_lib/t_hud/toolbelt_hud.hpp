#pragma once

#include <t_hud/texture.hpp>

namespace t_hud
{

class AgentToolbeltUI : public UIElement
{
    public:

    //static const float border = 16;       // border around entire panel
    static const float border = 0;       // border around entire panel
    static const float inc1 = 8; // spacing between slot icons
    static const float inc2 = 2;  // border around a slot icon

    static const float slot_size = 32;    // pixel dimension

    static const int xdim = 9;    // slot dimensions
    static const int ydim = 1;

    int width()
    {
        return xdim*slot_size + xdim*inc1 + inc2*2;
    }

    int height()
    {
        return ydim*slot_size + ydim*inc1 + inc2*2;
    }

    int selected_slot;

    void draw();

    int get_slot_at(int px, int py);

    void init()
    {
        assert(this->stack_numbers == NULL);
        
        // create HudText objects needed for stack rendering
        int max = xdim * ydim;
        this->stack_numbers = new HudText::Text[max];

        for (int i=0; i<max; i++)
        {
            HudText::Text* t = &this->stack_numbers[i];
            t->set_format((char*) "%d");
            t->set_format_extra_length(STACK_COUNT_MAX_LENGTH + 1 - 2);
            t->set_color(255,255,255,255);
            t->set_depth(-0.1f);
        }
    }

    AgentToolbeltUI()
    : selected_slot(0)
    {}
    
    ~AgentToolbeltUI()
    {
        if (this->stack_numbers != NULL) delete[] this->stack_numbers;
    }
};


int AgentToolbeltUI::get_slot_at(int px, int py)
{
    px -= xoff - border - inc1/2;
    py -= yoff + border + inc1/2;

    float width  = xdim*slot_size + (xdim-1)*inc1 + inc2*2;
    float height = ydim*slot_size + (ydim-1)*inc1 + inc2*2;

    if (px < 0 || px > width)  return NULL_SLOT;
    if (py < 0 || py > height) return NULL_SLOT;

    int xslot = px / (inc1 + slot_size);
    int yslot = py / (inc1 + slot_size);

    int slot = yslot * this->xdim + xslot;
    
    return slot;
}

void AgentToolbeltUI::draw()
{
    const float w = slot_size;

    glDisable(GL_DEPTH_TEST); // move render somewhere
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//#if 0
    //glColor4ub(0, 0, 50, 64);

    //{
        //float x = container_hud_x_off;
        //float y = container_hud_y_off;
        //float w = 2*border + xdim*slot_size+ (xdim-1)*inc1;
        //float h = 2*border + ydim*slot_size+ (ydim-1)*inc1;

        //glBegin(GL_QUADS);

        //glVertex2f(x, y+h, z);
        //glVertex2f(x+w, y+h ,z);
        //glVertex2f(x+w, y, z);
        //glVertex2f(x, y, z);

        //glEnd();

    //}
//#endif

    glBegin(GL_QUADS);
    glColor4ub(80, 80, 80, 128+64);
    for (int i=0; i<xdim; i++)
    for (int j=0; j<ydim; j++)
    {
        float x = xoff + border + i*(inc1+slot_size);
        float y = _yresf - (yoff + border + (j+1)*(inc1+slot_size));

        glVertex2f(x-inc2,y+w+inc2);
        glVertex2f(x+w+inc2, y+w+inc2);
        glVertex2f(x+w+inc2, y-inc2);
        glVertex2f(x-inc2, y-inc2);
    }

    glColor4ub(80, 80, 80, 128);

    for (int i=0; i<xdim; i++)
    for (int j=0; j<ydim; j++)
    {
        float x = xoff + border + i*(inc1+slot_size);
        float y = _yresf - (yoff + border + (j+1)*(inc1+slot_size));

        glVertex2f(x,y+w);
        glVertex2f(x+w, y+w);
        glVertex2f(x+w, y);
        glVertex2f(x, y);
    }

    // draw hover highlight
    glColor4ub(160, 160, 160, 128 + 64);
    int hover_slot = this->get_slot_at(mouse_x, mouse_y);
    if (hover_slot != NULL_SLOT)
    {
        int i = hover_slot % this->xdim;
        int j = hover_slot / this->xdim;
        
        float x = xoff + border + i*(inc1+slot_size);
        float y = _yresf - (yoff - border + (j+1)*(inc1+slot_size));

        glVertex2f(x,y+w);
        glVertex2f(x+w, y+w);
        glVertex2f(x+w, y);
        glVertex2f(x, y);
    }

    glEnd();

    glColor4ub(255, 255, 255, 255);
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, ItemSheetTexture );

    int* slot_types = Item::get_container_ui_types(this->container_id);
    Item::ItemContainer* container = Item::get_container(this->container_id);
    if (container == NULL) return;
    ItemID* items = container->slot;
    //int* slot_stacks = Item::get_container_ui_stacks(this->container_id);
    if (slot_types == NULL) return;
    //assert(slot_stacks != NULL);

    glBegin(GL_QUADS);

    for (int i=0; i<xdim; i++)
    for (int j=0; j<ydim; j++)
    {
        int slot = j * xdim + i;
        if (slot_types[slot] == NULL_ITEM_TYPE) continue;
        int tex_id = Item::get_sprite_index_for_type(slot_types[slot]);
        const float x = xoff + border + i*(inc1+slot_size);
        const float y = _yresf - (yoff + border + (j+1)*(inc1+slot_size));

        //const float iw = 8.0f; // icon_width
        //const int iiw = 8; // integer icon width
        const float iw = 16.0f; // icon_width
        const int iiw = 16; // integer icon width
        
        const float tx_min = (1.0/iw)*(tex_id % iiw);
        const float ty_min = (1.0/iw)*(tex_id / iiw);
        const float tx_max = tx_min + 1.0/iw;
        const float ty_max = ty_min + 1.0/iw;

        glTexCoord2f( tx_min, ty_min );
        glVertex2f(x,y+w);

        glTexCoord2f( tx_max, ty_min );
        glVertex2f(x+w, y+w );
            
        glTexCoord2f( tx_max, ty_max );
        glVertex2f(x+w, y);

        glTexCoord2f( tx_min, ty_max );
        glVertex2f(x, y);
    }

    glEnd();

    glEnable(GL_DEPTH_TEST); // move render somewhere
    glDisable(GL_BLEND);

    glDisable(GL_TEXTURE_2D);
    
    // draw border highlight
    if (this->selected_slot != NULL_SLOT)
    {   
        int slotx = this->selected_slot % xdim;
        int sloty = ydim - (this->selected_slot / xdim);
        const float x = xoff + border + slotx*(inc1+slot_size);
        const float y = _yresf - (yoff + border + (ydim-sloty+1)*(inc1+slot_size));

        const float b = inc2;

        glColor4ub(0, 0, 128+64, 255);
        glLineWidth(2.0);

        glBegin(GL_LINES);

        glVertex2f(x-b, y+w+b);
        glVertex2f(x+w+b, y+w+b);

        glVertex2f(x+w+b, y+w+b);
        glVertex2f(x+w+b, y-b);

        glVertex2f(x+w+b, y-b);
        glVertex2f(x-b, y-b);

        glVertex2f(x-b, y-b);
        glVertex2f(x-b, y+w+b);

        glEnd();

        glLineWidth(1.0);
    }

    glColor4ub(255, 255, 255, 255);

    /*
     * Draw stack numbers
     */

    HudFont::start_font_draw();
    const int font_size = 12;
    HudFont::set_properties(font_size);
    HudFont::set_texture();

    HudText::Text* text;
    for (int i=0; i<this->xdim; i++)
    for (int j=0; j<this->ydim; j++)
    {
        const int slot = j * this->xdim + i;
        //int count = slot_stacks[slot];
        Item::Item* item = Item::get_item(items[i]);
        if (item == NULL) continue;
        int count = item->durability;
        if (count <= 1) continue;
        assert(count < 1000); // the string is only large enough to hold "99"
        
        float x = xoff + border + i*(inc1+slot_size);
        x += slot_size - font_size/2;
        float y = _yresf - (yoff + border + (j+1)*(inc1+slot_size));
        y += font_size/2;

        text = &this->stack_numbers[slot];
        text->update_formatted_string(1, count);
        text->set_position(x,y);
        text->draw();
    }
    HudFont::reset_default();
    HudFont::end_font_draw();
}


}

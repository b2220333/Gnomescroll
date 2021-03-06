/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

const int LUDICROUS_REZ = 4000; // fixme once players start using resolutions over this...... if not much sooner  8)

namespace Hud
{

class MeterGraphic
{
    public:

        struct Quadticle
        {
            bool alive;
            int x, y;
            int x_speed, y_speed;
            int y_dest; // destination, the point where it disappears

            Quadticle() :
                alive(false), x(0), y(0), x_speed(0), y_speed(0), y_dest(0)
            {}
        };

        typedef enum
        {
            METANCH_RIGHT,
            METANCH_LEFT
        } MeterAnchor;

    MeterGraphic() :
        prev_w(0), num_qticles(0)
    {}

    void draw(int x, int y, int w, int h, float ratio)
    {
        this->draw(x,y,w,h,ratio, METANCH_LEFT);
    }

    void draw(int x, int y, int w, int h, float ratio, MeterAnchor anchor, bool is_dmg_meter=false)
    {
        float s_x = x;
        float s_y = y;
        float s_w = w;  // spectrum width (that a full meter would cover)
        float s_h = h;
        float dyn_w = s_w * ratio; // dynamic width (the current size of the meter itself)
        float lo = s_w - dyn_w; // dynamic leftover

        if      (anchor == METANCH_LEFT)
        {
            ;
        }
        else if (anchor == METANCH_RIGHT)
        {
            s_x += lo;
        }
        else // floats in center of meter spectrum
        {
            s_x += lo / 2;
        }

        draw_bound_texture(s_x, s_y, dyn_w, s_h);

        if (is_dmg_meter) // do particle stuff
        {
            // spawn for every pixel of the difference between current and previous meter
            for (int i = s_x + prev_w; i < s_x + dyn_w; i++)
            {
                //spawn_quadticle(
                //  i,
                //  s_y,
                //  randrange(-4, 4), // x_speed
                //  randrange(-7, -31),
                //  s_y - s_h - 240);
            }
            prev_w = dyn_w;

            update_and_draw_alive_particles(s_h);
        }
    }

    void update_and_draw_alive_particles(int h)
    {
        int num_alive_tallied = 0;
        for (int i = 0; /*num_alive_tallied < num_qticles &&*/ i < LUDICROUS_REZ; i++)
        {
            if (!quadticles[i].alive) continue;

            quadticles[i].x += quadticles[i].x_speed;
            quadticles[i].y += quadticles[i].y_speed;
            if (quadticles[i].y < quadticles[i].y_dest)
            {
                quadticles[i].alive = false;
                num_qticles--;
            }
            else
            {
                num_alive_tallied++;
                draw_bound_texture(
                    quadticles[i].x,
                    quadticles[i].y, 1, h);
            }
        }
    }

    bool spawn_quadticle(int x, int y, int x_speed, int y_speed, int y_dest)
    {
        for (int i = 0; i < LUDICROUS_REZ; i++)
        {
            if (!quadticles[i].alive)
            {
                quadticles[i].x = x;
                quadticles[i].y = y;
                quadticles[i].x_speed = x_speed;
                quadticles[i].y_speed = y_speed;
                quadticles[i].y_dest = y_dest;
                quadticles[i].alive = true;
                num_qticles++;
                return true;
            }
        }

        return false;
    }

    private:
        float prev_w; // previous dynamic width

        // blink related
        //bool blink_on;
        //// handle blinking
        //ticks_til_blink--;
        //if (ticks_til_blink < 1)
        //{
        //  if (green_to_red > .75f) ticks_til_blink = slowest_blink_delay;
        //  blink_on = !blink_on;
        //}
        //if (blink_on) {       }

        // quadticles
        int num_qticles; // number of alive quadticles
        Quadticle quadticles[LUDICROUS_REZ];
};

}   // Hud

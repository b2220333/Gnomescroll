const int LUDICROUS_REZ = 4000; // fixme once players start using resolutions over this...... if not much sooner  8)

typedef enum {
    METANCH_RIGHT,
    METANCH_LEFT
} MeterAnchor;

struct Quadticle {
	bool Alive;
	int x, y, x_speed, y_speed, y_dest; // destination, the point where it disappears

	Quadticle() {
		Alive = false;
		x = y = x_speed = y_speed = y_dest = 0;
	}
};



namespace Hud {
	class JetPackMeter {
		int prev_w; // previous width
		int num_qticles; // number of alive quadticles
		Quadticle quadticles[LUDICROUS_REZ]; 



	public:
		JetPackMeter() {
			prev_w = 0;
			num_qticles = 0;
		}

		void draw(float _xresf, float _yresf, PlayerAgent_state* pa, MeterAnchor anchor = METANCH_RIGHT) {
			float s_w = _xresf/4; // spectrum width (that a full bar would cover)
			float s_x = 0;        // spectrum x pos
			float s_h = _yresf/64;
			float s_y = 0;

			float dyn_w = s_w * (float)pa->jetpack.fuel / JETPACK_FUEL_MAX; // dynamic width (the current size of the meter itself)
			float lo = s_w - dyn_w; // dynamic leftover
			
			if        (anchor == METANCH_LEFT) {
				;
			} else if (anchor == METANCH_RIGHT) {
				s_x += lo;
			} else { // floats in center of meter spectrum
				s_x += lo / 2;
			}

			// draw
			glEnable(GL_BLEND);
			glColor4ub(255,255,255,115);
			draw_bound_texture(s_x, s_y, dyn_w, s_h);
			for (int i = 0; i < prev_w - dyn_w; i++) {
				// FIXME   spawn_quadticles, uses the dynamic leftover width i think?  or SHOULD?
				//spawn_quadticle( // ..... somewhere random within current bar dimensions
				//	randrange(s_x,   s_x +   (dyn_w <= 0)   ?   1   :   dyn_w), 
				//	s_y,
				//	randrange(-2, 2), 
				//	randrange(7, 11), 
				//	s_y + s_h + 40);
			}
			prev_w = dyn_w;

			// health meter
			//Agent* a = pa->you;
			//if (a != NULL)
			//{
			//	w = _xresf * (float)a->status.health / (float)a->status.health_max;
			//	draw_bound_texture(	(_xresf - w) / 2, _yresf-h*9, w, h);
			//}



			// update & draw alive particles
			int num_alive_tallied = 0;
			for (int i = 0; /*num_alive_tallied < num_qticles &&*/ i < LUDICROUS_REZ; i++) {
				if (quadticles[i].Alive) {
					quadticles[i].x += quadticles[i].x_speed;
					quadticles[i].y += quadticles[i].y_speed;

					if (quadticles[i].y > quadticles[i].y_dest) {
						quadticles[i].Alive = false;
						num_qticles--;
					} else {
						num_alive_tallied++;

						draw_bound_texture(
							quadticles[i].x, 
							quadticles[i].y, s_h, s_h);
					}
				}
			}

		}



		
		bool spawn_quadticle(int x, int y, int x_speed, int y_speed, int y_dest) {
			for (int i = 0; i < LUDICROUS_REZ; i++) {
				if (!quadticles[i].Alive) {
					quadticles[i].x = x;
					quadticles[i].y = y;
					quadticles[i].x_speed = x_speed;
					quadticles[i].y_speed = y_speed;
					quadticles[i].y_dest = y_dest;
					quadticles[i].Alive = true;
					num_qticles++;
					return true;
				}
			}

			return false;
		}
	};
}
#pragma once

#if DC_CLIENT
dont_include_this_file_in_client
#endif

#include <t_gen/twister.hpp>
#include <t_map/t_map.hpp>



namespace t_gen {
    void make_art_gallery(int z_start) {
        printf("Making art gallery\n");

		//t_map::set(i, 0, x_start+a, (CubeID)i);
        //t_map::get_cube_id("rock"); 
		
		int span = 7;
		int max_columns = 6;

        int num_valid = 0;
        for (int i = 0; i < MAX_CUBES; i++) if (t_map::isValidCube((CubeID)i) ) num_valid++;
		int row = 0;
		int col = 0;
        for (int i = 0; i < MAX_CUBES; i++) {
			if (t_map::isValidCube((CubeID)i) ) {
				t_gen::set_region(col*span,  row*span,  z_start, 
					span,  span,  1, (CubeID)i);
				t_gen::set_region(col*span+2,row*span+2,z_start+1, 
					3,3,1, (CubeID)i);

				t_map::set(col*span+3,row*span+3,z_start+4, (CubeID)i);
				col++;
				if (col >= max_columns) {
					col = 0;
					row++;
				}
			}
		}
    }
}   // t_gen
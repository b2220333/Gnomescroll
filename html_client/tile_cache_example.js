function tile_cache_canvas() {
	
	//needs init code
	//will re-init the cache when board is re-sized
	
	cache_canvas_dom = ??? // the thing used for reading/writing to canvas
	
	canvas_tile_width = 16;  //width of canvas in number of tiles
	canvas_tile_height = 16; //width of canvas in number of tiles
	
	tile_pixel_width = 20;
	tile_pixel_width = 20;
	
}

function tile_cache() {

	tile_cache_canvas = ??? // assign an instance
		
	cache_dict = []; //maps tile_id to number, [tile_cache_position, x_offset, y_offset, x_width, y_width]
	//contains the data needed for copying a tile from cache canvas to the board canvas
	
	tileset_metadata = ??? //stores the rendering metadata returned by the get_tiles info command
	
	board_canvas // canvas that we are drawing to
	
	function draw_tile(board_x, board_y, tile_id) { //takes the x,y position and id of tile type to draw
		
		//check to see if tile is in cache, if not; add to cache
		if(! tile_id in cache_dict) {
			//tile not in cache, draw tile into cache
			
			//tile drawing properies are symbol, symbol_color, and background color
			
			/*
			temp = tileset_metadata.get_tile_drawing_properties(tile_id) ;
			// returns [symbol_num, [symbol_color_r, symbol_color_g, symbol_color_b], [background_color_r, background_color_g, background_color_b]]
			symbol = temp[0];
			symbol_color = temp[1];
			background_color = temp[2];
			 */
			
			//hardcode for now, but get drawing properties from tileset_data eventually
			symbol = 1; 
			symbol_color = [256, 256, 256] //rgb
			background_color = [100, 0, 0] //rgb
			
			//use square (0,0) as temporary drawing canvas
			
			//draw background color onto (x,y)
			
			x_row = 
			y_row = 
			
			x_offset = 
			
		}
		
		//copy tile from the tile_cache onto the board
		
		
		
	}
	
}

//this stores tile
function tileset_metadata() {
	
	tile_drawing_property_array = [];
	
	function get_tile_drawing_properties(tile_id) {
		return tile_drawing_property_array[tile_id];
	}
	
	function set_tile_drawing_properties(tile_id, symbol, symbol_r, symbol_g, symbol_b, background_r, background_g, background_b) {
		tile_drawing_property_array[tile_id] = [symbol, [symbol_r, symbol_g, symbol_b], [backgrond_r, background_g, background_b]];
	}
}

import pyglet
from pyglet.gl import *

from cube_dat import CubeGlobal
from world_state import WorldStateGlobal

from player import Player

base_dir = "./"

import random

class World():

    terrainMap = None
    mapChunkManager = None

    def init(self):
        self.terrainMap = WorldStateGlobal.terrainMap
        self.mapChunkManager = CubeGlobal.mapChunkManager
        CubeGlobal.setTextureGrid(self.texture_grid)

    def __init__(self):
        tile_image = pyglet.image.load(base_dir + 'texture/textures_01.png')
        tile_image_grid = pyglet.image.ImageGrid(tile_image, 16, 16)
        tile_texture_grid = pyglet.image.TextureGrid(tile_image_grid)
        self.texture_grid = tile_texture_grid
        self.texture_grid_mipmap = tile_image.get_mipmapped_texture()

        self.players = []
        self.mipmap = 6
        self.gl_smooth = 0

        self.init()

    def toggle_mipmap(self):
        self.mipmap = (self.mipmap+1) % 7
        print "mipmap= %i" % self.mipmap

    def toggle_gl_smooth(self):
        self.gl_smooth = (self.gl_smooth+1) % 2

    def tick(self):
        self.mapChunkManager.update_chunk()
        pass

    def draw(self):
        self.draw_chunk()
        self.draw_players()

    def test_chunk(self):
        print "Start chunk generation"
        x_max = 64
        y_max = 64
        z_max = 64
        for xa in range(0, x_max):
            for ya in range(0, y_max):
                for za in range(0, z_max):
                    rnd = random.randint(0,64)
                    if rnd < 16:
                        rnd2 = random.randint(1,4)
                        self.terrainMap.set(xa,ya,za, rnd2)
                        self.mapChunkManager.set_map(xa,ya,za)
        print "Finished chunk generation"

    def draw_chunk(self):
        if self.gl_smooth == 0:
            glShadeModel(GL_FLAT)
        else:
            glShadeModel(GL_SMOOTH);

        glEnable(GL_CULL_FACE);
        glEnable(self.texture_grid.target)

        _mipmap = self.mipmap
        if _mipmap == 0:
            glBindTexture(self.texture_grid.target, self.texture_grid.id)
        elif _mipmap == 1:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        elif _mipmap == 2:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        elif _mipmap == 3:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        elif _mipmap == 4:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        elif _mipmap == 5:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        elif _mipmap == 6:
            glBindTexture(self.texture_grid_mipmap.target, self.texture_grid_mipmap.id)
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        #for mct in self.mct_array.values():
        #    if mct.empty == False:
        #        mct.vertexList.draw(pyglet.gl.GL_QUADS)

        self.mapChunkManager.draw_batch.draw()

        glShadeModel(GL_SMOOTH); #the default
        glDisable(GL_CULL_FACE)
        glDisable(self.texture_grid.target)
        #self.draw_players()

    def draw_point(self, x, y, r, g, b):
        z=0
        pyglet.graphics.draw(1, pyglet.gl.GL_POINTS,
            ('v3f', (x, y, z)),
            ('c3B', (r, g, b)) )

    def add_player(self, player =None):
        if Player == None:
            self.players.append(Player())
        else:
            self.players.append(player)

    def draw_players(self):
        for p in self.players:
            p.draw()

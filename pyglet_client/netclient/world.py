import pyglet
from pyglet.gl import *

base_dir = "./"

import random

class World():

    terrainMap = None
    mapChunkManager = None

    def init(self):
        self.terrainMap = GameStateGlobal.terrainMap
        self.mapChunkManager = MapChunkManagerGlobal.mapChunkManager
        CubeGlobal.setTextureGrid(self.texture_grid)

    def __init__(self):
        tile_image = pyglet.image.load(base_dir + 'texture/textures_01.png')
        tile_image_grid = pyglet.image.ImageGrid(tile_image, 16, 16)
        tile_texture_grid = pyglet.image.TextureGrid(tile_image_grid)
        self.texture_grid = tile_texture_grid
        self.texture_grid_mipmap = tile_image.get_mipmapped_texture()

        self.players = []
        self.agents = []
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
        for p in GameStateGlobal.projectileList.values():
            p.tick()
        pass

    def draw(self):
        self.draw_chunk()
        self.draw_agents()
        self.draw_projectiles()

        MapChunkManagerGlobal.transparentBlockManager.update_all_blocks()
        MapChunkManagerGlobal.transparentBlockManager.update_vbo()
        tmp = MapChunkManagerGlobal.transparentBlockManager.vertexList
        if tmp != None:
            tmp.draw()

    def draw_projectiles(self):
        projectiles.draw_projectiles()

    def test_chunk(self):
        print "Start chunk generation"
        x_max = 64
        y_max = 64
        z_max = 16
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

        #transparency
        #glEnable (GL_BLEND);
        #glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        #end transparency

        glEnable(GL_CULL_FACE);
        glEnable(self.texture_grid.target)

        ##choose mipmapping option
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

        self.mapChunkManager.draw_batch.draw()

        #transparency
        #glDisable(GL_BLEND);

        glShadeModel(GL_SMOOTH); #the default
        glDisable(GL_CULL_FACE)
        glDisable(self.texture_grid.target)

    def add_player(self, player):
        self.players.append(player)

    def add_agent(self, agent=None):
        self.agents.append(agent)

    def draw_agents(self):
        for agent in GameStateGlobal.agentList.values():
            if not agent.dead:
                agent.draw()

import projectiles

from cube_dat import CubeGlobal
from game_state import GameStateGlobal
from map_chunk_manager import MapChunkManagerGlobal

from player import Player

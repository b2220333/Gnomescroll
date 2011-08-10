'''
Toys, miscellaneous objects
'''

from game_objects import DetachableObject, StaticObject, filter_props, TeamItem

from random import randint as rand

def rand_spot(z=15):
    return (rand(5, 15), rand(5, 15), z)

class Flag(DetachableObject, TeamItem):

    def __init__(self, id, radius, team, *args, **kwargs):
        print "CREATE FLAG"
        DetachableObject.__init__(self, id, radius)
        TeamItem.__init__(self, team, own=False, other=True)
        self.auto_grab = True
        self.drop_on_death = True
        self.team = team
        self.type = 1
        self._set_name()
        self.spawn()

    def spawn(self):
        xyz = rand_spot()
        self.pos(xyz)
        self.on_ground = True
        self.owner = None

    def take(self, new_owner):
        if TeamItem.can_take(self, new_owner):
            DetachableObject.take(self, new_owner)

    def json(self, properties=None):
        if properties is None:
            d = DetachableObject.json(self)
            d.update({
                'team'     :   self.team.id,
            })
        else:
            d = filter_props(obj, properties)
        return d        


class Base(StaticObject, TeamItem):

    def __init__(self, id, team, *args, **kwargs):
        print 'CREATING BASE'
        StaticObject.__init__(self, id)
        TeamItem.__init__(self, team, False, False)
        self.spawned = False
        self.team = team
        self.type = 2
        self._set_name()
        self.spawn()
        
    def spawn(self):
        if not self.spawned:
            self.state[0:3] = rand_spot(z=2)
            print 'SPAWNING BASE at %s' % (self.state,)
            self.spawned = True

    def json(self, properties=None):
        if properties is None:
            d = StaticObject.json(self)
            d.update({
                'team'  :   self.team.id,
            })
        else:
            d = filter_props(obj, properties)
        return d

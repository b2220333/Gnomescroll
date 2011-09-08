

cdef extern from "c_lib.c":
    int init_c_lib()

#print "Initing c_lib"

_init = 0
def init():
    global _init
    if _init == 0:
        init_c_lib()
    _init = 1

## net stuff

cdef extern from "./net_lib/server.h":
    void _NetServerInit()
    void _NetServerTick()

def NetServerInit():
    _NetServerInit()

def NetServerTick():
    _NetServerTick()

##timer

cdef extern from "../c_lib/time/physics_timer.h":
    int _start(int frequency)
    int _tick_check()
    long _get_time()
    long _get_tick()
    void _START_CLOCK()
    int _GET_TICK()

def StartPhysicsTimer(frequency):
    _start(frequency)

def PhysicsTimerTickCheck():
    return _tick_check()

def get_time():
    return _get_time()

def get_tick():
    return _get_tick()

def START_CLOCK():
    _START_CLOCK()

def GET_TICK():
    return _GET_TICK()

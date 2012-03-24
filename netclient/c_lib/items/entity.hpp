#pragma once

#include <c_lib/common/enum_types.hpp>
#include <c_lib/template/object_list.hpp>

namespace ItemDrops
{

// TODO
// new folder, namespace for this crap
// organize the templates
// separate the ObjectState/Data stuff from the rest

struct ObjectData
{
    Object_types type;
    //TODO:
    // fill the struct with object metadata
    // all objects' initialization data will be stored in an array
    // when creating an object, array lookup will occur for data, and fill in
    // avoids some massive switch statements
    // can even be inherited into ObjectState
    // and make all properties class members

    // these members should const/immutable for most purposes
    // but they should be adjustable by 1 thing: the dat loader
};

// encapsulates all information needed for any object
class ObjectState
{
    public:
    int id;
    Object_types type;
    bool blow_up_on_death;
    bool pickup;
    float pickup_radius;
    int ttl;
    int ttl_max;
    float damp;
    VerletParticle* vp;
    float mass;
    float texture_scale;
    int texture_index;
    bool broadcast_death;
    int picked_up_by;
    
    ObjectState()
    : id(-1), type(OBJ_TYPE_NONE), blow_up_on_death(false), pickup(false),
    pickup_radius(1.0f), ttl(0), ttl_max(1), damp(1.0f), vp(NULL),
    mass(1.0f), texture_scale(1.0f), texture_index(0),
    broadcast_death(false), picked_up_by(-1)
    {}

    ~ObjectState()
    {
        if (this->vp != NULL)
            delete this->vp;
    }

    // TODO: setters
};

// encapsulates all information needed for any object
template <class Subclass>
class ObjectStateTemplate: public ObjectState
{
    public:
    Subclass* object;
    ObjectStateTemplate<Subclass>() {}
};

template <class Super, typename State>
class TickCombiner: public Super
{
    public:
        inline void tick(State* state)
        {Super::tick(state);} __attribute((always_inline));
};

template <typename State>
class TickAnchor
{
    public:
        inline void tick(State* state) {/*Empty*/} __attribute((always_inline));
};

template <class Super, typename State>
class DrawCombiner: public Super
{
    public:
        inline void draw(State* state)
        {Super::draw(state);} __attribute((always_inline));
};

template <typename State>
class DrawAnchor
{
    public:
        inline void draw(State* state) {/*Empty*/} __attribute((always_inline));
};

template <class Super, typename State>
class BornCombiner: public Super
{
    public:
        inline void born(State* state)
        {Super::born(state);} __attribute((always_inline));
};


template <typename State>
class BornAnchor
{
    public:
        inline void born(State* state) {/*Empty*/} __attribute((always_inline));
};

template <class Super, typename State>
class DieCombiner: public Super
{
    public:
        inline void die(State* state)
        {Super::die(state);} __attribute((always_inline));
};

template <typename State>
class DieAnchor
{
    public:
        inline void die(State* state) {/*Empty*/} __attribute((always_inline));
};

template <class Super, typename State>
class DieBlowup: public Super
{
    public:
        inline void die(State* state)
        {Super::die(state);} __attribute((always_inline));
};

template <class Super, typename State>
class DiePickup: public Super
{
    public:
        inline void die(State* state) __attribute((always_inline));
};

template <class Super, typename State>
class BornPickup: public Super
{
    public:
        inline void born(State* state) __attribute((always_inline));
};

#define NoTick(STATE) TickAnchor<STATE>
#define NoDraw(STATE) DrawAnchor<STATE>
#define NoBorn(STATE) BornAnchor<STATE>
#define NoDie(STATE) DieAnchor<STATE>

class ObjectPolicyInterface
{
    public:
        virtual void tick() = 0;
        virtual void draw() = 0;
        virtual void born() = 0;
        virtual void die() = 0;
        virtual ObjectState* state() = 0;
};

template
<
    class Wrapper,
    class TickSuper,
    class DrawSuper,
    class BornSuper,
    class DieSuper
>
class ObjectPolicy:
public TickCombiner<TickSuper, ObjectStateTemplate<Wrapper> >,
public DrawCombiner<DrawSuper, ObjectStateTemplate<Wrapper> >,
public BornCombiner<BornSuper, ObjectStateTemplate<Wrapper> >,
public DieCombiner<DieSuper, ObjectStateTemplate<Wrapper> >,
public ObjectPolicyInterface
{
    public:
        ObjectStateTemplate<Wrapper> _state;
    
    void tick() { TickCombiner<TickSuper, ObjectStateTemplate<Wrapper> >::tick(&this->_state); }
    void draw() { DrawCombiner<DrawSuper, ObjectStateTemplate<Wrapper> >::draw(&this->_state); }
    void born() { BornCombiner<BornSuper, ObjectStateTemplate<Wrapper> >::born(&this->_state); }
    void die() { DieCombiner<DieSuper, ObjectStateTemplate<Wrapper> >::die(&this->_state); }
    ObjectState* state() { return &this->_state; }

    ObjectPolicy<Wrapper, TickSuper, DrawSuper, BornSuper, DieSuper>(Wrapper* wrapper)
    {
        _state.object = wrapper;    // pointer to subclass
    }
};


template <class Super, typename State>
class TickParticle: public Super
{
    public:
    inline void tick(State* state) __attribute((always_inline));
};

template <class Super, typename State>
class TickTTL: public Super
{
    public:
    inline void tick(State* state) __attribute((always_inline));
};

template <class Super, typename State>
class TickPickup: public Super
{
    public:
    inline void tick(State* state) __attribute((always_inline));
};

template <class Super, typename State>
class DrawBillboardSprite: public Super
{
    public:
    inline void draw(State* state) __attribute((always_inline));
};

class GameObject_list: public Object_list<ObjectPolicyInterface, GAME_OBJECTS_MAX>
{
    public:
        const char* name() { return "GameObject"; }
        
        void tick();
        void draw();
        ObjectPolicyInterface* create(float x, float y, float z, float mx, float my, float mz, Object_types type);
        void destroy(int id);
        
        GameObject_list() { print(); }
};

// TODO: restructure the list creation to adapt based on type

} // ItemDrops

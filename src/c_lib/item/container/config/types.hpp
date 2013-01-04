#pragma once

#include <item/common/constants.hpp>

typedef void (*send_decision) (ContainerActionType, ItemContainerID, int);
                                 // action, container, slot
#if DC_CLIENT
typedef ContainerActionType (*decision_tree) (ItemContainerID, int);
#endif                                 // container, slot
#if DC_SERVER
typedef ContainerActionType (*decision_tree) (AgentID, ClientID, ItemContainerID, int);
#endif                              // agent, client, container, slot

typedef ItemContainer::ItemContainerInterface* (*containerCreate) (ItemContainerType, ItemContainerID);

namespace ItemContainer
{
// forward declaration
bool is_valid_container_name(const char* name);
}

class ContainerAttributes
{
    public:
        ItemContainerType type;
        
        bool loaded;

        char name[DAT_NAME_MAX_LENGTH+1];

        int xdim, ydim;
        int alt_xdim, alt_ydim;
        bool attached_to_agent;

        decision_tree alpha_action;
        decision_tree beta_action;
        send_decision alpha_packet;
        send_decision beta_packet;
        
        // alt actions (synth purchase, craft output etc)
        decision_tree alpha_action_alt;
        decision_tree beta_action_alt;
        send_decision alpha_packet_alt;
        send_decision beta_packet_alt;

        containerCreate create_function;

    void set_name(const char* name)
    {
        IF_ASSERT(!ItemContainer::is_valid_container_name(name)) return;
        strcpy(this->name, name);
    }

    unsigned int max_dim()
    {
        return this->xdim * this->ydim;
    }
    
    int max_alt_dim()
    {
        return this->alt_xdim * this->alt_ydim;
    }
    
    ~ContainerAttributes()
    {
    }

    ContainerAttributes()
    {
        this->init();
    }

    void init()
    {
        this->loaded = false;
        memset(this->name, 0, sizeof(this->name));
        this->xdim = 0;
        this->ydim = 0;
        this->alt_xdim = 0;
        this->alt_ydim = 0;
        this->attached_to_agent = false;        
        this->alpha_action = NULL;
        this->beta_action = NULL;
        this->alpha_packet = NULL;
        this->beta_packet = NULL;
        this->alpha_action_alt = NULL;
        this->beta_action_alt = NULL;
        this->alpha_packet_alt = NULL;
        this->beta_packet_alt = NULL;
    }  
};

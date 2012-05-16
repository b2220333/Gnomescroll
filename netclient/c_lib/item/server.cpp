#include "server.hpp"

#if DC_CLIENT
dont_include_this_file_in_client
#endif

namespace Item
{

static void pack_item_create(Item* item, item_create_StoC* msg)
{
    assert(item->id != NULL_ITEM);
    assert(item->type != NULL_ITEM_TYPE);
    assert(item->group != NULL_ITEM_GROUP);
    assert(item->stack_size > 0);
    msg->id = item->id;
    msg->type = item->type;
    msg->group = item->group;
    msg->durability = item->durability;
    msg->stack_size = item->stack_size;
}

void broadcast_item_create(ItemID item_id)
{
    Item* item = get_item(item_id);
    if (item == NULL) return;
    item_create_StoC msg;
    pack_item_create(item, &msg);
    msg.broadcast();
}

Item* send_item_create(int client_id, ItemID item_id)
{
    Item* item = get_item(item_id);
    if (item == NULL) return NULL;
    item_create_StoC msg;
    pack_item_create(item, &msg);
    msg.sendToClient(client_id);
    return item;
}

void send_container_item_create(int client_id, ItemID item_id, int container_id, int slot)
{
    Item* item = send_item_create(client_id, item_id);
    if (item == NULL) return;
    send_container_insert(client_id, item_id, container_id, slot);
}


static void pack_item_state(Item* item, item_state_StoC* msg)
{
    msg->id = item->id;
    msg->stack_size = item->stack_size;
    msg->durability = item->durability;
    msg->energy = item->energy;
}

void send_item_state(int client_id, ItemID item_id)
{
    Item* item = item_list->get((ItemID)item_id);
    if (item == NULL) return;
    item_state_StoC msg;
    pack_item_state(item, &msg);
    msg.sendToClient(client_id);
}

void broadcast_item_state(ItemID item_id)
{
    Item* item = item_list->get((ItemID)item_id);
    if (item == NULL) return;
    item_state_StoC msg;
    pack_item_state(item, &msg);
    msg.broadcast();
}

void send_item_destroy(int client_id, ItemID item_id)
{
    item_destroy_StoC msg;
    msg.id = item_id;
    msg.sendToClient(client_id);
}

void broadcast_item_destroy(ItemID item_id)
{
    item_destroy_StoC msg;
    msg.id = item_id;
    msg.broadcast();
}

int auto_add_item_to_container(int client_id, int container_id, ItemID item_id)
{
    assert(container_id != NULL_CONTAINER);

    Item* item = get_item(item_id);
    assert(item != NULL);

    ItemContainerInterface* container = get_container(container_id);
    assert(container != NULL);

    int slot = container->get_stackable_slot(item->type, item->stack_size);
    if (slot == NULL_SLOT)
    {   // insert normal
        slot = container->get_empty_slot();
        if (slot == NULL_SLOT) return NULL_SLOT;
        container->insert_item(slot, item_id);
        send_container_item_create(client_id, item_id, container_id, slot);
    }
    else
    {   // stack
        ItemID slot_item_id = container->get_item(slot);
        assert(slot_item_id != NULL_ITEM);
        merge_item_stack(item_id, slot_item_id);
        send_item_state(client_id, slot_item_id);
        send_container_insert(client_id, slot_item_id, container_id, slot);
    }
    return slot;
}

bool agent_open_container(int agent_id, int container_id)
{
    assert(opened_containers != NULL);
    ASSERT_VALID_AGENT_ID(agent_id);
    assert(container_id != NULL_CONTAINER);

    ItemContainerInterface* container = get_container(container_id);
    if (container == NULL) return false;

    Agent_state* a = ServerState::agent_list->get(agent_id);
    if (a == NULL) return false;

    if (!container->can_be_opened_by(a->id)) return false;

    // relase currently opened container
    if (opened_containers[agent_id] != NULL_CONTAINER)
    {
        ItemContainerInterface* opened = get_container(opened_containers[agent_id]);
        assert(opened != NULL);
        opened->unlock(a->id);
        opened_containers[agent_id] = NULL_CONTAINER;
    }

    // place player lock on container if we want
    container->lock(a->id);
    opened_containers[agent_id] = container->id;

    // send container contents to player
    send_container_contents(a->id, a->client_id, container_id);
    return true;
}

void agent_close_container(int agent_id, int container_id)
{
    assert(opened_containers != NULL);
    ASSERT_VALID_AGENT_ID(agent_id);
    assert(container_id != NULL_CONTAINER);
    
    ItemContainerInterface* container = get_container(container_id);
    if (container == NULL) return;

    container->unlock(agent_id);
    opened_containers[agent_id] = NULL_CONTAINER;
}

void send_container_close(int agent_id, int container_id)
{
    ASSERT_VALID_AGENT_ID(agent_id);

    Agent_state* a = ServerState::agent_list->get(agent_id);
    if (a == NULL) return;
    
    close_container_StoC msg;
    msg.container_id = container_id;
    msg.sendToClient(a->client_id);
}

void send_container_open(int agent_id, int container_id)
{
    ASSERT_VALID_AGENT_ID(agent_id);

    Agent_state* a = ServerState::agent_list->get(agent_id);
    if (a == NULL) return;
    
    open_container_StoC msg;
    msg.container_id = container_id;
    msg.sendToClient(a->client_id);
}

void send_open_container_failed(int client_id, int container_id, int event_id)
{
    open_container_failed_StoC msg;
    msg.container_id = container_id;
    msg.event_id = event_id;
    msg.sendToClient(client_id);
}

}   // Item

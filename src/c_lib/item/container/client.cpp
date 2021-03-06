/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#include "client.hpp"

#if DC_SERVER
# error Do not include this file in the server
#endif

namespace ItemContainer
{

static uint16_t container_event_id = 0;
ItemContainerID* container_event = NULL;

int record_container_event(ItemContainerID container_id)
{
    container_event_id += 1;
    container_event_id %= CONTAINER_EVENT_MAX;
    container_event[container_event_id] = container_id;
    return container_event_id;
}

void send_container_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_container_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_synthesizer_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    synthesizer_container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD || action == PURCHASE_ITEM_FROM_SYNTHESIZER)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_synthesizer_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    synthesizer_container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_craft_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    craft_container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_purchase_item_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action != PURCHASE_ITEM_FROM_SYNTHESIZER) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    purchase_item_from_synthesizer_action_CtoS msg;
    msg.event_id = container_event_id;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.send();
}

void send_craft_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    craft_container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_craft_item_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(action != CRAFT_ITEM_FROM_BENCH) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;

    record_container_event(container_id);

    craft_item_from_bench_action_CtoS msg;
    msg.event_id = container_event_id;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.send();
}

void send_no_container_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;

    record_container_event(NULL_CONTAINER);

    no_container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;
    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();
    msg.send();
}

void send_no_container_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    record_container_event(NULL_CONTAINER);

    no_container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;
    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();
    msg.send();
}

void send_smelter_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    smelter_container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_smelter_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    smelter_container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_crusher_alpha_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    crusher_container_action_alpha_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_crusher_beta_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(player_hand_ui == NULL) return;
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    crusher_container_action_beta_CtoS msg;
    msg.event_id = container_event_id;
    msg.action = action;

    msg.container_id = container_id;
    msg.slot = slot;

    msg.hand_type = player_hand_ui->get_item_type();
    msg.hand_stack = player_hand_ui->get_item_stack();

    if (action == FULL_HAND_TO_WORLD)
    {
        msg.slot_type = NULL_ITEM_TYPE;
        msg.slot_stack = 1;
    }
    else
    {
        ItemContainerUIInterface* container = get_container_ui(container_id);
        IF_ASSERT(container == NULL) return;

        msg.slot_type = container->get_slot_type(slot);
        msg.slot_stack = container->get_slot_stack(slot);
    }

    msg.send();
}

void send_crusher_crush_action(ContainerActionType action, ItemContainerID container_id, int slot)
{
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    IF_ASSERT(action == CONTAINER_ACTION_NONE) return;

    record_container_event(container_id);

    crusher_crush_item_CtoS msg;
    msg.event_id = container_event_id;

    msg.container_id = container_id;
    msg.send();
}

// Handlers

void mouse_left_click_handler(ItemContainerID container_id, int slot, bool alt_action)
{
    class Agents::Agent* you = ClientState::player_agent.you();
    if (you == NULL) return;
    if (you->status.dead) return;

    ItemContainerType container_type = get_container_type(container_id);
    class ContainerAttribute* attr = get_attr(container_type);
    IF_ASSERT(attr == NULL) return;
    GS_ASSERT(attr->loaded);

    ContainerActionType action = CONTAINER_ACTION_NONE;
    if (alt_action)
    {
        IF_ASSERT(attr->alpha_action_alt == NULL) return;
        action = attr->alpha_action_alt(container_id, slot);
    }
    else
    {
        IF_ASSERT(attr->alpha_action == NULL) return;
        action = attr->alpha_action(container_id, slot);
    }

    if (action == CONTAINER_ACTION_NONE) return;

    if (alt_action)
    {
        IF_ASSERT(attr->alpha_packet_alt == NULL) return;
        attr->alpha_packet_alt(action, container_id, slot);
    }
    else
    {
        IF_ASSERT(attr->alpha_packet == NULL) return;
        attr->alpha_packet(action, container_id, slot);
    }
}

void mouse_right_click_handler(ItemContainerID container_id, int slot, bool alt_action)
{
    class Agents::Agent* you = ClientState::player_agent.you();
    if (you == NULL) return;
    if (you->status.dead) return;

    ItemContainerType container_type = get_container_type(container_id);
    class ContainerAttribute* attr = get_attr(container_type);
    IF_ASSERT(attr == NULL) return;
    GS_ASSERT(attr->loaded);

    ContainerActionType action = CONTAINER_ACTION_NONE;
    if (alt_action)
    {
        IF_ASSERT(attr->beta_action_alt == NULL) return;
        action = attr->beta_action_alt(container_id, slot);
    }
    else
    {
        IF_ASSERT(attr->beta_action == NULL) return;
        action = attr->beta_action(container_id, slot);
    }

    if (action == CONTAINER_ACTION_NONE) return;

    if (alt_action)
    {
        IF_ASSERT(attr->beta_packet_alt == NULL) return;
        attr->beta_packet_alt(action, container_id, slot);
    }
    else
    {
        IF_ASSERT(attr->beta_packet == NULL) return;
        attr->beta_packet(action, container_id, slot);
    }
}

void send_container_open(ItemContainerID container_id, int event_id)
{
    IF_ASSERT(event_id < 0 || container_id == NULL_CONTAINER) return;
    open_container_CtoS msg;
    msg.container_id = container_id;
    msg.event_id = event_id;
    msg.send();
}

void send_container_close(ItemContainerID container_id)
{
    IF_ASSERT(container_id == NULL_CONTAINER) return;
    close_container_CtoS msg;
    msg.container_id = container_id;
    msg.send();
}

}   // ItemContainer

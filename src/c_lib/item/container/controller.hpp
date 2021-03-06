/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

#include <agent/constants.hpp>

#include <item/common/enum.hpp>
#include <item/common/constants.hpp>
#include <item/properties.hpp>

namespace ItemContainer
{

#if DC_CLIENT
// transactions
ContainerActionType alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType beta_action_decision_tree(ItemContainerID id, int slot);

ContainerActionType synthesizer_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType synthesizer_beta_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType synthesizer_shopping_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType synthesizer_shopping_beta_action_decision_tree(ItemContainerID id, int slot);

ContainerActionType craft_input_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType craft_input_beta_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType craft_output_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType craft_output_beta_action_decision_tree(ItemContainerID id, int slot);

ContainerActionType no_container_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType no_container_beta_action_decision_tree(ItemContainerID id, int slot);

ContainerActionType smelter_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType smelter_beta_action_decision_tree(ItemContainerID id, int slot);

ContainerActionType crusher_alpha_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType crusher_beta_action_decision_tree(ItemContainerID id, int slot);
ContainerActionType crusher_crush_alpha_action_decision_tree(ItemContainerID id, int slot);
#endif

#if DC_SERVER
// transactions
ContainerActionType alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);

ContainerActionType synthesizer_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType synthesizer_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType synthesizer_shopping_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType synthesizer_shopping_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);

ContainerActionType craft_input_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType craft_input_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType craft_output_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType craft_output_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);

ContainerActionType no_container_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID container_id, int slot);
ContainerActionType no_container_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID container_id, int slot);

ContainerActionType smelter_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType smelter_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);

ContainerActionType crusher_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType crusher_beta_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
ContainerActionType crusher_crush_alpha_action_decision_tree(AgentID agent_id, ClientID client_id, ItemContainerID id, int slot);
#endif

}   // ItemContainer

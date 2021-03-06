/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

namespace HudContainer
{

const int STACK_COUNT_MAX_LENGTH = 11;

typedef enum
{
    UI_ELEMENT_NONE = 0,
    UI_ELEMENT_AGENT_INVENTORY,
    UI_ELEMENT_AGENT_TOOLBELT,
    UI_ELEMENT_SYNTHESIZER_CONTAINER,
    UI_ELEMENT_CRAFTING_CONTAINER,
    UI_ELEMENT_STORAGE_BLOCK,
    UI_ELEMENT_SMELTER,
    UI_ELEMENT_ENERGY_TANKS,
    UI_ELEMENT_CRUSHER,
    UI_ELEMENT_CACHE,
    UI_ELEMENT_EQUIPMENT,
} UIElementType;

} // HudContainer

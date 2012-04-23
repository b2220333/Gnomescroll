#pragma once

/* Inventory Property, to use in an object */

#include <c_lib/items/inventory/constants.hpp>

#if DC_CLIENT
#include <c_lib/items/inventory/client.hpp>
#endif

#if DC_SERVER
#include <c_lib/items/inventory/server.hpp>
#endif

#include <c_lib/items/inventory/packets.hpp>
#include <c_lib/items/inventory/list.hpp>
#include <c_lib/items/inventory/interface.hpp>

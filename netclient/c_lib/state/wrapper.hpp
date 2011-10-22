#include <c_lib/state/server_state.hpp>
#include <c_lib/state/client_state.hpp>

#include <c_lib/agent/agent.hpp>

Agent_state* create_agent();
Agent_state* create_agent(int id);

Agent_state* get_agent(int id);
void delete_agent(int id);
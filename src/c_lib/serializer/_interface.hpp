#pragma once

#if GS_SERIALIZER

namespace serializer
{

void init();
void teardown();
void update();

bool load_data();

// defined in map.cpp
bool load_map(const char* filename);
void save_map();
void wait_for_threads();

// defined in players.hpp
bool save_player(UserID user_id, AgentID agent_id);
bool save_player_container(ClientID client_id, int container_id);

int begin_player_load(UserID user_id, ClientID client_id);
bool load_player_container(int player_load_id, ItemContainerType container_type); 
bool end_player_load(int player_load_id);

// Called from outside this module. NetPeerManager is the only thing that knows about true failure
void player_load_failed();

bool create_player_container_items_from_data(AgentID agent_id, int* containers, int n_containers);

void save_containers();

// This is only for the serializer -- it handles the backup copy logic
bool save_file(const char* fn, const char* fn_tmp, const char* fn_bak);

}   // serializer

#else

// stubs
namespace serializer
{

void init();
void teardown();
void update();

// these map functions are actually enabled for non-serializer mode
extern bool should_save_map;
bool load_map(const char* filename);
void save_map();
bool load_default_map();
bool save_file(const char* fn, const char* fn_tmp, const char* fn_bak);


save_containers() {}
void check_save_state() {}

bool save_player(UserID user_id, AgentID agent_id) { return true; }
bool save_player_container(ClientID client_id, int container_id) { return true; }

int begin_player_load(UserID user_id, ClientID client_id) { return 0; }
bool load_player_container(int player_load_id, ItemContainerType container_type) { return true; }
bool end_player_load(int player_load_id) { return true; }

void player_load_failed() {}

bool create_player_container_items_from_data(AgentID agent_id, int* containers, int n_containers) { return true; }

}   // serializer

#endif

#pragma once

/*
 * All client agent input triggers should be implemented here
 */

class PlayerAgent_state;// forward declare

class PlayerAgent_action {
    private:
        PlayerAgent_state* p;
        
    public:
        void fire();
        void hitscan_laser();
        void hitscan_pick();
        void set_block();
        void throw_grenade();

        bool switch_weapon(int i);
        void reload();
        int select_block();
        
        void place_spawner();
        void place_turret();

        void remove_item_from_inventory();  // TEMP (real method will take slot index)
        void add_item_to_inventory();

        explicit PlayerAgent_action(PlayerAgent_state* player_agent);
};

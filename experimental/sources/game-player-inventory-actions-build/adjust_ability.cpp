#include "inventory_actions.hpp"
using namespace norrath_inventory_actions_detail;

extern "C" void norrath_inventory_adjust_ability(int player_index,int ability_index) {
    const Address record=player_record(player_index);
    const unsigned initial_budget=read_byte_fresh(record+240u);
    const Address ability=record+120u+unsigned(ability_index)*4u;
    if(initial_budget!=0u) {
        const unsigned value=read_word_fresh(ability);
        write_word_ordered(ability,value+1u);
        // Ability storage may overlap the budget; the native LBU is fresh
        // after SW, and ADDIU255 followed by SB decrements modulo256.
        const unsigned updated_budget=read_byte_fresh(record+240u);
        write_byte_ordered(record+240u,updated_budget+255u);
    }
    // Neither a zero initial budget nor an update skips this actual call.
    if(norrath_net_player_is_local(player_index)!=0) {
        void* sequence=norrath_lump_find_resource(
            text(address(norrath_inventory_ability_sound_lump_view)),
            text(address(norrath_inventory_ability_sound_resource_view)));
        norrath_sound_play_sequence(sequence,nullptr,1.0f);
    }
    // Real localization/resource/audio calls may change the record.
    if(read_byte_fresh(record+240u)==0u) {
        write_byte_ordered(record+240u,0u);
        write_byte_ordered(record+242u,1u);
    }
    norrath_player_recompute_ability_modifiers();
}

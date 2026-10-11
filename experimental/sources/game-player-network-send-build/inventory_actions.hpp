#pragma once
#include "network_local.hpp"
extern "C" {
// Complete actual roots. Void effects-only entry points do not promise the
// incidental original V0 left by their final genuine external calls.
void norrath_inventory_adjust_ability(int player_index,int ability_index);
void norrath_inventory_switch_dual_wield(int player_index,const void* description);
int norrath_inventory_item_check_slot(int player_index,const void* description);
// Genuine callees remain actual services, without fabricated handlers/responses.
int norrath_sound_play_sequence(void* animation_header,void* object,float gain);
void norrath_player_recompute_ability_modifiers();
void norrath_net_send_message(int handler,unsigned char* payload,int length);
void norrath_player_reset_animations(int player_index);
void norrath_inventory_set_animation(int player_index,void* animation_state,int mode);
// Unsized genuine data views: no initialized objects or runtime owners.
extern unsigned char norrath_inventory_ability_sound_lump_view[];
extern const unsigned char norrath_inventory_ability_sound_resource_view[];
extern unsigned char norrath_inventory_message_handler_word_view[];
extern unsigned char norrath_inventory_animation_state_base_view[];
}
namespace norrath_inventory_actions_detail {
using namespace norrath_skill_progression_detail;
inline unsigned read_byte_fresh(Address a){return *static_cast<const volatile unsigned char*>(bytes(a));}
inline void write_byte_ordered(Address a,unsigned value){*static_cast<volatile unsigned char*>(bytes(a))=static_cast<unsigned char>(value);}
}

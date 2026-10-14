#pragma once
#include "sound_restore_update.hpp"
#include "sound_native_registers.hpp"

extern "C" {
// Complete genuine private LOCAL packet helpers; no original-global aliases.
void* norrath_sound_rpc_get_packet(void* pool_state);
void norrath_sound_rpc_free_packet(void* packet);
// True unfinished SDK/kernel services, never answers or original code bindings.
int norrath_sound_disable_interrupts();
int norrath_sound_enable_interrupts();
void norrath_sound_write_back_data_cache(void* address,int bytes);
int norrath_sound_send_command(int command,void* packet,int packet_bytes,
                              void* source,void* destination,int data_bytes);
int norrath_sound_create_semaphore(void* parameters);
int norrath_sound_delete_semaphore(int semaphore);
// Same original WaitSema16 and signature as the reviewed kernel source.
int norrath_player_wait_semaphore(int semaphore);
extern unsigned char norrath_sound_rpc_packet_pool_state_view[];
}
namespace norrath_sound_rpc_detail {
using namespace norrath_sound_restore_update_detail;
// The exact shared typed always_inline nativeGP getter is used directly at the
// original capture point. Only its actual low32 word is published to client24.
}

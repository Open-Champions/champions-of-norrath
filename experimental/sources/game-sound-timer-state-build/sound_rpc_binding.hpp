#pragma once
#include "sound_rpc.hpp"
#include "sound_startup.hpp"

extern "C" {
// Same canonical actual Remote/IOP/startup entries, complete new algorithms.
int norrath_sound_bind_rpc(void* client,int server_id,int mode);
void norrath_sound_initialize_rpc(int mode);

// Complete original service identities stay real, unfinished code imports.
void norrath_sound_initialize_command();
using NorrathSoundCommandHandler = void (*)(void* packet,void* parameter);
void norrath_sound_add_command_handler(int command,NorrathSoundCommandHandler handler,
                                      void* parameter);
// GetReg SYSCALL122 compares the whole physical V0; GetSreg's signed LW
// returns all64 transported bits. No host register cache or answer is supplied.
unsigned long long norrath_sound_get_register(int register_id);
unsigned long long norrath_sound_get_system_register(int register_id);
void norrath_sound_set_register(int register_id,unsigned value);

// Four distinct exact LOCAL original code identities. Real command dispatch
// supplies packet/parameter in A0/A1. These have no compiled provider yet.
void norrath_sound_rpc_request_end(void* packet,void* parameter);
void norrath_sound_rpc_request_bind(void* packet,void* parameter);
void norrath_sound_rpc_request_call(void* packet,void* parameter);
void norrath_sound_rpc_request_rdata(void* packet,void* parameter);

// Existing pool view from sound_rpc.hpp is reused unchanged. No array, struct,
// allocation, reset or owned initialization is defined by these declarations.
extern unsigned char norrath_sound_rpc_initialized_word_view[];
extern unsigned char norrath_sound_rpc_packet_buffer_view[];
extern unsigned char norrath_sound_rpc_free_buffer_view[];
extern unsigned char norrath_sound_rpc_free_buffer2_view[];
}
static_assert(sizeof(NorrathSoundCommandHandler)==4,
              "fixed native EE function pointer transport is32 bits");
namespace norrath_sound_rpc_binding_detail {
using namespace norrath_sound_rpc_detail;
inline unsigned uncached_pointer_word(Address address_word) {
    // Actual original OR bit29 before publishing the three state pointers.
    // This is an address-bit operation, not a synthesized linker/memory alias.
    return address_word|0x20000000u;
}
}

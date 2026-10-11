#pragma once
#include "sound_rpc_binding.hpp"
extern "C" {
void* norrath_sound_rpc_get_free_packet(void* pool_state);
void* norrath_sound_rpc_get_free_packet_indexed(void* pool_state,int index);
void* norrath_sound_rpc_find_server(unsigned server_id,void* pool_state);

// These are genuine, currently unfinished interrupt SDK services. Full V0 is
// retained where the original tests the physical register, rather than a
// fabricated low32 success code. The six ordinary inputs occupy A0..T1.
unsigned long long norrath_sound_interrupt_send_command(
    int command,void* packet,int packet_bytes,void* source,
    void* destination,int data_bytes);
unsigned long long norrath_sound_interrupt_wake_thread(int thread_id);

// Private machine-slot callback transport, not a guessed timer structure.
// AlarmHandler preserves the unused A1/A2 slots, supplies a handle in A0 and
// the captured user pointer in A3. Both actual retry bodies ignore A0..A2.
// Its full V0 test distinguishes retry delay2048 from zero.
using NorrathSoundRpcRetryHandler=unsigned long long (*)(
    unsigned long long slot_a0,unsigned long long slot_a1,
    unsigned long long slot_a2,void* parameter);
unsigned long long norrath_sound_rpc_retry_bind(
    unsigned long long,unsigned long long,unsigned long long,void*);
unsigned long long norrath_sound_rpc_retry_rdata(
    unsigned long long,unsigned long long,unsigned long long,void*);
unsigned long long norrath_sound_interrupt_set_timer_alarm(
    unsigned long long delay,NorrathSoundRpcRetryHandler handler,
    void* parameter);
}
static_assert(sizeof(unsigned long long)==8 && sizeof(NorrathSoundRpcRetryHandler)==4,
              "fixed EE raw64 register slots and32-bit function pointers");
namespace norrath_sound_rpc_callbacks_detail {
using namespace norrath_sound_rpc_binding_detail;
}

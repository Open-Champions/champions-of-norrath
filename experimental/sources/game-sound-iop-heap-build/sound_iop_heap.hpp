#pragma once
#include "sound_startup.hpp"
#include "sound_restore_update.hpp"

extern "C" {
int norrath_sound_initialize_iop_heap();
// The allocation/query declarations and true nine-argument RPC declaration
// are the unchanged canonical interfaces in the copied sound headers.
extern unsigned char norrath_sound_iop_heap_bind_word_view[];
extern unsigned char norrath_sound_iop_heap_client_view[];
extern unsigned char norrath_sound_iop_heap_receive_view[];
extern unsigned char norrath_sound_iop_heap_send_view[];
}

namespace norrath_sound_iop_heap_detail {
using namespace norrath_sound_sequence_control_detail;
// Actual observed word addresses are aligned4. These are fresh native-width
// accesses to unowned address views; they do not allocate or initialize state.
inline unsigned read_aligned_word_fresh(Address a) {
    return *reinterpret_cast<const volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline void write_aligned_word(Address a,unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a))=value;
}
__attribute__((always_inline)) inline void five_wait_nops() {
    asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop":::"memory");
}
}

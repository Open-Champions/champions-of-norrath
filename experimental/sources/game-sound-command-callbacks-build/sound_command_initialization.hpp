#pragma once
#include "sound_register_semaphore.hpp"
extern "C" {
// Existing whole canonical declarations initialize_command()/add_command_handler
// and NorrathSoundCommandHandler are inherited unchanged from the Binding header.
// Genuine distinct LOCAL builtin functions, unfinished source imports only.
void norrath_sound_command_change_address(void* packet,void* parameter);
void norrath_sound_command_set_system_register(void* packet,void* parameter);
// Entire actual DMA callback ignores incoming argument registers and returns0.
// Kernel-supplied extra lanes are unknown, never seeded or given guessed values.
using NorrathSoundCommandDmaHandler=int (*)();
int norrath_sound_command_dma_handler();
// True whole original kernel/SDK identities with actual three explicit lanes.
unsigned long long norrath_sound_add_dma_handler(int channel,
                                                NorrathSoundCommandDmaHandler handler,
                                                int next_handler);
unsigned long long norrath_sound_enable_dma(int channel);
void norrath_sound_set_dma_chain();
// Actual ELF symbol observations only: all views are imported/unsized/unowned.
extern unsigned char norrath_sound_command_initialized_word_view[];
extern unsigned char norrath_sound_command_packet_buffer_view[];
extern unsigned char norrath_sound_command_send_buffer_view[];
extern unsigned char norrath_sound_command_initialization_packet_view[];
extern unsigned char norrath_sound_command_dma_handle_view[];
extern unsigned char norrath_sound_command_state_view[];
extern unsigned char norrath_sound_command_system_handlers_view[];
// Genuine soft_reg view is inherited; no owned initializer or capacity is added.
}
static_assert(sizeof(NorrathSoundCommandDmaHandler)==4,"actual EE codeword transport32");
namespace norrath_sound_command_initialization_detail {
using namespace norrath_constructor_detail;
using norrath_sound_native_register_detail::current_gp_bits;
using Raw64=unsigned long long;
// Genuine aligned32 volatile effects preserve the observed native word widths.
// Mapped aligned word/object lifetime is a precondition, not an owner/capacity.
__attribute__((always_inline)) inline unsigned read_aligned_word(Address a) {
    return *reinterpret_cast<const volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a));
}
__attribute__((always_inline)) inline void write_aligned_word(Address a,unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a))=value;
}
inline unsigned command_handler_word(NorrathSoundCommandHandler handler) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler));
}
}

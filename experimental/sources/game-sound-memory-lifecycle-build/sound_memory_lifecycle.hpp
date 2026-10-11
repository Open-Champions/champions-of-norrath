#pragma once
#include "skill_progression.hpp"
#include "heap.hpp"
#include "reentrant_free.hpp"

extern "C" {
void norrath_sound_assert(const char* file,unsigned line,const char* condition,int captured_ra);
void norrath_animation_state_deallocate(void* allocation);
void norrath_mem_free_wrapper(void* allocation);
void norrath_mem_free(void* allocation);
void norrath_player_free_plain(void* allocation);
extern unsigned char norrath_assert_context_word_view[];
extern const unsigned char norrath_assert_format_view[];
extern unsigned char norrath_enable_malloc_wrapper_byte_view[];
}
namespace norrath_sound_memory_lifecycle_detail {
using namespace norrath_skill_progression_detail;
inline const char* text_at(Address a) {return reinterpret_cast<const char*>(bytes(a));}
inline unsigned read_byte_fresh(Address a) {
    return *static_cast<const volatile unsigned char*>(bytes(a));
}
// The original assertion executes an actual word store of zero at address0.
// This typed memory primitive preserves that architectural access; it supplies
// no fabricated failure object, mapping, handler, address alias or permission.
inline void native_store_zero_word(Address destination) {
    asm volatile("sw $zero,0(%0)"::"r"(destination):"memory");
}
}


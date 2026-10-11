#pragma once
using NorrathSoundCacheWord=unsigned;
using NorrathSoundCacheRaw64=unsigned long long;
static_assert(sizeof(NorrathSoundCacheWord)==4 && sizeof(int)==4 &&
              sizeof(void*)==4 && sizeof(__UINTPTR_TYPE__)==4 &&
              sizeof(NorrathSoundCacheRaw64)==8,"fixed EE word/pointer32 and native GPR64");
extern "C" void norrath_sound_write_back_data_cache(void* address,int bytes);
namespace norrath_sound_cache_writeback_detail {
using Word=NorrathSoundCacheWord;
using Raw64=NorrathSoundCacheRaw64;
inline Word address_word(const void* value) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(value));
}
inline Raw64 canonical_line_operand(Word value) {
    // Actual aligned native register within ordinary sign-extended pointer32
    // input scope. Arbitrary noncanonical incoming high GPR bits are unproved.
    return Raw64(value)|((value&0x80000000u)?0xffffffff00000000ull:0ull);
}
__attribute__((always_inline)) inline void native_sync() {
    asm volatile("sync" : : : "memory");
}
template<int Offset>
__attribute__((always_inline)) inline void native_cache_writeback(Raw64 base) {
    // Keep the same native base for each eight-block immediate displacement,
    // including address-boundary crossings. This is the actual CPU operation.
    asm volatile("cache 0x18,%0(%1)" : : "i"(Offset),"r"(base) : "memory");
}
__attribute__((always_inline)) inline void native_remainder_nop() {
    asm volatile("nop" : : : "memory");
}
}

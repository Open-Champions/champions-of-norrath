#pragma once
// Shared 32-bit views for the task scheduler. Address arithmetic wraps at
// 32 bits. A host check supplies a base so those low bits name real memory.
static_assert(sizeof(unsigned) == 4, "32-bit scheduler words");
static_assert(sizeof(unsigned long long) == 8, "64-bit pair loads");

extern "C" {
// Observed nmTasks at 0x00484478, 4 bytes, initially zero. Original binding
// is local. This global is the linkable name other units reference.
extern unsigned char nmTasks[4];
// Observed currentTask at 0x0048447c, 4 bytes, initially zero. Local binding
// in the image; same linkable-name choice as nmTasks.
extern unsigned char currentTask[4];
// Observed taskList at 0x00793e60, local BSS, symbol size 0. The next label,
// colorStack, is 512 bytes later. That distance is not a size field.
extern unsigned char taskList[];
void enginePrintTasks__Fv();
#if defined(NORRATH_SCHEDULER_HOST_CHECK)
extern unsigned long long norrath_scheduler_host_address_base;
void norrath_scheduler_host_print_tick();
void norrath_scheduler_host_call(unsigned word);
#endif
}

namespace norrath_scheduler {
using Word = unsigned;
using Pair = unsigned long long;

inline Word low_address(const void* pointer) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline Word low_address(void (*pointer)()) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
inline unsigned char* from_low(Word location) {
#if defined(NORRATH_SCHEDULER_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_scheduler_host_address_base + location);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(location));
#endif
}
inline Word load_word(Word location) {
    const volatile unsigned char* in = from_low(location);
    const Word b0 = in[0];
    const Word b1 = in[1];
    const Word b2 = in[2];
    const Word b3 = in[3];
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}
inline Pair load_pair(Word location) {
    const Pair low = load_word(location);
    const Pair high = load_word(location + 4u);
    return low | (high << 32);
}
inline void store_word(Word location, Word value) {
    volatile unsigned char* out = from_low(location);
    out[0] = static_cast<unsigned char>(value);
    out[1] = static_cast<unsigned char>(value >> 8);
    out[2] = static_cast<unsigned char>(value >> 16);
    out[3] = static_cast<unsigned char>(value >> 24);
}
inline void store_pair(Word location, Pair value) {
    store_word(location, static_cast<Word>(value));
    store_word(location + 4u, static_cast<Word>(value >> 32));
}
inline bool signed_less(Word left, Word right) {
    return (left ^ 0x80000000u) < (right ^ 0x80000000u);
}
// Signed <= 0, including the high-bit values blez treats as negative.
inline bool nonpositive(Word value) {
    return value == 0u || (value & 0x80000000u) != 0u;
}
}

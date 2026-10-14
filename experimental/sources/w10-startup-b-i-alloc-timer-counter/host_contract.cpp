/* Host contract. Not a plan input. Counter records live in a host arena addressed by
 * 32-bit offsets. */
#define NORRATH_HOST_CONTRACT 1
#include "i_alloc_timer_counter.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
alignas(16) static unsigned char arena[4096];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" { TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1}; }
static Counter& rec(unsigned at) { return *reinterpret_cast<Counter*>(arena + at); }
int main() {
    int bad = 0;
    // Empty chain: the sentinel and no stores.
    if (iAllocTimerCounter() != static_cast<int>(0x80008005u) || g_Timer.generation != 1 || g_Timer.allocated != 0) { std::puts("empty chain wrong"); ++bad; }
    // Chain of three records at 0x100, 0x140, 0x180; dirty the fields to see them cleared.
    for (unsigned at : {0x100u, 0x140u, 0x180u}) { std::memset(arena + at, 0xa5, 64); }
    rec(0x100).next = 0x140; rec(0x140).next = 0x180; rec(0x180).next = 0;
    g_Timer.free_counters = 0x100;
    unsigned expect_gen = 1;
    for (unsigned at : {0x100u, 0x140u, 0x180u}) {
        const unsigned next = rec(at).next;
        const int got = iAllocTimerCounter();
        ++expect_gen;
        const unsigned word8 = ((expect_gen << 1) & 0x3ffu) | 1u;
        if (static_cast<unsigned>(got) != ((at << 4) | word8)) { std::printf("handle wrong for %x: %x\n", at, got); ++bad; }
        if (rec(at).generation_word != word8 || rec(at).flags != 0 || rec(at).handler != 0 || rec(at).word_24 != 0) { std::puts("record fields wrong"); ++bad; }
        if (rec(at).word_16 != 0xa5a5a5a5a5a5a5a5ull || rec(at).word_32 != 0xa5a5a5a5u || rec(at).unused_4 != 0xa5a5a5a5u) { std::puts("untouched fields changed"); ++bad; }
        if (g_Timer.free_counters != next || g_Timer.generation != expect_gen || g_Timer.allocated != expect_gen - 1) { std::puts("g_Timer wrong"); ++bad; }
    }
    // The generation word wraps in ten bits.
    g_Timer.generation = 0x1ff; rec(0x100).next = 0; g_Timer.free_counters = 0x100;
    if ((iAllocTimerCounter() & 0x3ff) != 1) { std::puts("wrap wrong"); ++bad; }
    if (bad) return 1;
    std::puts("iAllocTimerCounter contract passed");
    return 0;
}

#include "sound_iop_heap.hpp"
using namespace norrath_sound_iop_heap_detail;

extern "C" int norrath_sound_initialize_iop_heap() {
    for (;;) {
        const int status=norrath_sound_bind_rpc(
            bytes(address(norrath_sound_iop_heap_client_view)),
            signed_word(0x80000003u),0);
        if (status<0) return -1;
        if (read_aligned_word_fresh(address(norrath_sound_iop_heap_client_view)+36u)!=0u) {
            write_aligned_word(address(norrath_sound_iop_heap_bind_word_view),0u);
            return 0;
        }
        // Original decrements before each five-NOP group, including the final
        // unsigned -1 group. No wait precedes the first genuine bind attempt.
        unsigned remaining=0x100000u;
        do {
            remaining-=1u;
            five_wait_nops();
        } while (remaining!=0xffffffffu);
    }
}

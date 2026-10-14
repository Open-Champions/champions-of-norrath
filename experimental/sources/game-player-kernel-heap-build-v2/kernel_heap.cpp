#include "kernel_heap.hpp"
using namespace norrath_kernel_heap_detail;

extern "C" Wide norrath_player_thread_id_bits() {
    return syscall_without_argument(47u);
}
extern "C" int norrath_player_wait_semaphore(int semaphore) {
    return signed_word(Word(syscall_with_argument(68u,semaphore)));
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    return signed_word(Word(syscall_with_argument(66u,semaphore)));
}
extern "C" Wide norrath_player_heap_end_bits() {
    return syscall_without_argument(62u);
}
extern "C" unsigned char* norrath_player_errno_pointer() {
    return bytes(read_word(address(norrath_factory_impure_pointer_word)));
}

extern "C" Wide norrath_player_adjust_break(int increment) {
    const bool interrupts_were_enabled=(read_status()&0x10000u)!=0u;
    if(interrupts_were_enabled) {
        do {
            disable_interrupts_and_synchronize();
        } while((read_status()&0x10000u)!=0u);
    }
    const Word heap_word=address(norrath_player_heap_pointer_word);
    const Word next=read_word(heap_word)+Word(increment);
    const Wide limit=norrath_player_heap_end_bits();
    // Original ADDU and both LW values are sign extended in the 64-bit GPR.
    const Word old=read_word(heap_word);
    if(limit<extend_signed_word(next)) {
        unsigned char* error=norrath_player_errno_pointer();
        write_word(address(error),12u);
        if(interrupts_were_enabled)enable_interrupts();
        return 0xffffffffffffffffull;
    }
    write_word(heap_word,next);
    if(interrupts_were_enabled)enable_interrupts();
    return extend_signed_word(old);
}

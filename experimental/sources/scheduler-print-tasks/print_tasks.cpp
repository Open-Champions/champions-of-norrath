#include "../scheduler-access/access.hpp"

// Observed enginePrintTasks at 0x00191738, 52 bytes. It loads nmTasks once.
// A positive count counts a register down to zero. A nonpositive count
// returns. Nothing is stored, and no text is printed.
extern "C" void enginePrintTasks__Fv() {
    using namespace norrath_scheduler;
    Word count = load_word(low_address(nmTasks));
    if (nonpositive(count)) return;
    // An empty register countdown is deleted at -O2. The volatile copy keeps
    // the same number of decrements in the compiled body.
    volatile Word remaining = count;
    do {
        remaining -= 1u;
#if defined(NORRATH_SCHEDULER_HOST_CHECK)
        norrath_scheduler_host_print_tick();
#endif
    } while (remaining != 0u);
}

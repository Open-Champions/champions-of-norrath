#include "../scheduler-access/access.hpp"

// Observed engineRunTasks at 0x00191770, 188 bytes.
// currentTask is stored as 0 before the count test, including when the count
// is nonpositive. Each record's flags word is at byte 12. The callback runs
// when (flags & mask) == 0 and either select is -1 or (flags & select) != 0.
// After a callback, count and currentTask are loaded again and the next index
// is that current value plus one. A skipped record uses index + 1 instead.
// The index is stored to currentTask at the end of every iteration, including
// the iteration that fails the bound. The callback is called with no
// arguments. The address register still holding index * 16 is incidental.
extern "C" void engineRunTasks__Fii(int mask, int select) {
    using namespace norrath_scheduler;
    const Word count_at = low_address(nmTasks);
    const Word current_at = low_address(currentTask);
    Word count = load_word(count_at);
    store_word(current_at, 0u);
    if (nonpositive(count)) return;

    Word index = 0u;
    const Word list = low_address(taskList);
    const Word mask_word = static_cast<Word>(mask);
    const Word select_word = static_cast<Word>(select);
    while (signed_less(index, count)) {
        const Word row = list + (index << 4);
        const Word flags = load_word(row + 12u);
        Word next;
        if ((flags & mask_word) != 0u ||
            (select_word != 0xffffffffu && (flags & select_word) == 0u)) {
            next = index + 1u;
        } else {
            const Word callback = load_word(row);
#if defined(NORRATH_SCHEDULER_HOST_CHECK)
            norrath_scheduler_host_call(callback);
#else
            reinterpret_cast<void (*)()>(static_cast<__UINTPTR_TYPE__>(
                reinterpret_cast<__UINTPTR_TYPE__>(from_low(callback))))();
#endif
            count = load_word(count_at);
            next = load_word(current_at) + 1u;
        }
        index = next;
        store_word(current_at, index);
    }
}

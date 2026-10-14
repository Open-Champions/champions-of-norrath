#include "../scheduler-access/access.hpp"

// Observed engineRemoveTask at 0x00191648, 240 bytes.
// It scans callbacks while the captured count stays positive. A miss reads
// currentTask and returns. A hit rewinds currentTask when current >= index,
// copies each later record down one 16-byte slot, stores count-1, then calls
// enginePrintTasks. A negative count does not scan; index 0 is then treated
// as the hit because it is not equal to that negative count.
extern "C" void engineRemoveTask__FPFv_v(void (*callback)()) {
    using namespace norrath_scheduler;
    const Word count = load_word(low_address(nmTasks));
    Word index = 0u;
    if (!nonpositive(count)) {
        const Word list = low_address(taskList);
        if (load_word(list) != low_address(callback)) {
            index = 1u;
            Word slot = list;
            while (signed_less(index, count)) {
                slot += 16u;
                if (load_word(slot) == low_address(callback)) break;
                index += 1u;
            }
        }
    }

    const Word current_at = low_address(currentTask);
    const Word current = load_word(current_at);
    if (index == count) return;

    if (!signed_less(current, index))
        store_word(current_at, current - 1u);
    const Word last = count - 1u;
    if (signed_less(index, last)) {
        Word cursor = low_address(taskList) + (index << 4);
        for (Word remaining = last - index; remaining != 0u;) {
            remaining -= 1u;
            const Pair first = load_pair(cursor + 16u);
            const Pair second = load_pair(cursor + 24u);
            store_pair(cursor, first);
            store_pair(cursor + 8u, second);
            cursor += 16u;
        }
    }
    store_word(low_address(nmTasks), count - 1u);
    enginePrintTasks__Fv();
}

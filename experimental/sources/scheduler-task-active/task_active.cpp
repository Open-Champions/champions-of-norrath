#include "../scheduler-access/access.hpp"

// Observed engineTaskActive at 0x001915c0, 132 bytes.
// It compares each record's text pointer, at byte 8, with strcmp. The count
// is loaded again after each mismatch. A match returns 1. A nonpositive
// count, or a scan that passes the reloaded count, returns 0.
extern "C" int strcmp(const char* left, const char* right);

extern "C" int engineTaskActive__FPc(char* name) {
    using namespace norrath_scheduler;
    Word count = load_word(low_address(nmTasks));
    if (nonpositive(count)) return 0;
    Word index = 0u;
    Word text_at = low_address(taskList) + 8u;
    for (;;) {
        const Word text_word = load_word(text_at);
        text_at += 16u;
        if (strcmp(reinterpret_cast<char*>(from_low(text_word)), name) == 0) return 1;
        count = load_word(low_address(nmTasks));
        index += 1u;
        if (!signed_less(index, count)) return 0;
    }
}

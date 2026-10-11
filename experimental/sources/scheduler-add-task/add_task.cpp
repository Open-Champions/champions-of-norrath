#include "../scheduler-access/access.hpp"

// Observed engineAddTask at 0x00191490, 300 bytes.
// Records are 16 bytes: callback, signed key, text pointer, flags.
// A positive count rejects a callback already stored. Otherwise the new
// record is inserted so a strictly smaller key moves later records up one
// slot. Equal keys stay ahead of the new record. The captured current index
// advances when it is >= the insertion slot. Count advances by one from the
// value captured on entry. enginePrintTasks runs after those stores.
// The callback is stored, not called. Negative counts skip the scan and the
// shift and still write at the wrapped count*16 offset.
extern "C" void engineAddTask__FPFv_viPci(
    void (*callback)(), int key, char* text, int flags) {
    using namespace norrath_scheduler;
    const Word count_at = low_address(nmTasks);
    const Word count = load_word(count_at);
    const Word callback_word = low_address(callback);
    const Word key_word = static_cast<Word>(key);
    const Word text_word = low_address(text);
    const Word list = low_address(taskList);

    if (!nonpositive(count)) {
        Word candidate = list;
        for (Word index = 0u; signed_less(index, count); index += 1u) {
            if (load_word(candidate) == callback_word) return;
            candidate += 16u;
        }
    }

    Word insertion = count;
    Word offset = count << 4;
    Word destination = list + offset;
    while (!nonpositive(insertion)) {
        if (!signed_less(key_word, load_word(destination - 12u))) break;
        insertion -= 1u;
        const Pair first = load_pair(destination - 16u);
        const Pair second = load_pair(destination - 8u);
        store_pair(destination, first);
        store_pair(destination + 8u, second);
        destination -= 16u;
        offset -= 16u;
    }

    const Word current_at = low_address(currentTask);
    const Word current = load_word(current_at);
    const Word entry = list + offset;
    store_word(entry + 4u, key_word);
    store_word(entry, callback_word);
    store_word(entry + 8u, text_word);
    store_word(entry + 12u, static_cast<Word>(flags));
    if (!signed_less(current, insertion))
        store_word(current_at, current + 1u);
    store_word(count_at, count + 1u);
    enginePrintTasks__Fv();
}

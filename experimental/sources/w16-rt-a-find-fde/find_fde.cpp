// Observed find_fde at 0x002fc8c8, 424 bytes, a local routine of the exception runtime. It returns the
// frame-description entry covering a code address, searching the registered object records chained from the
// global list head `objects` (0x008eab18). Record layout, 32-bit words:
//    0 first address covered     4 end address     8 entry list     12 sorted entry-pointer array (0 until set up)
//   16 element count of that array     20 next record
// The routine holds the kernel semaphore whose id is the word at __sce_eh_sema_id (0x00444a34): WaitSema first,
// SignalSema on every exit path (on the binary-search path it is released before the search starts).
//   * Records are visited from the head. A record whose first address is 0 is set up with frame_init (0x002fc760)
//     and its first address is reread. The first record with first <= pc < end is used; none gives 0.
//   * If the array word is 0, or equals the entry-list word, the record is set up with frame_init and both words
//     are reread.
//   * Array word 0: the result is search_fdes (0x002fc708) of the entry list and pc.
//   * Array word equal to the entry-list word: that word is treated as a zero-terminated sequence of lists, each
//     passed to search_fdes in turn; the first nonzero answer is the result, else 0.
//   * Otherwise the array is binary searched without the semaphore: lo = 0, hi = count (0 returns 0), mid =
//     (lo + hi) >> 1; an entry whose word 2 is above pc moves hi to mid; one with pc < word2 + word3 is returned;
//     otherwise lo = mid + 1.
// Inferred: the array is ordered by start address. Unknown: the bodies of frame_init and search_fdes, and how
// records are registered. Addresses are 32-bit words; a host check can supply an address base.
#if defined(W16_HOST_CHECK)
extern unsigned char* w16_host_arena;
template <typename T> static inline T* at(unsigned word) {
    return word == 0 ? static_cast<T*>(nullptr) : reinterpret_cast<T*>(w16_host_arena + word);
}
#else
template <typename T> static inline T* at(unsigned word) {
    return reinterpret_cast<T*>(static_cast<__UINTPTR_TYPE__>(word));
}
#endif

namespace {
struct Record {
    unsigned first;
    unsigned end;
    unsigned entries;
    unsigned array;
    unsigned count;
    unsigned next;
};
}

extern "C" {
extern int __sce_eh_sema_id;
extern unsigned objects;
int WaitSema(int id);
int SignalSema(int id);
void frame_init(void* record);
unsigned search_fdes(unsigned entries, unsigned pc);

unsigned find_fde(unsigned pc) {
    WaitSema(__sce_eh_sema_id);
    Record* record = at<Record>(objects);
    while (record != nullptr) {
        if (record->first == 0) frame_init(record);
        if (pc >= record->first && pc < record->end) break;
        record = at<Record>(record->next);
    }
    if (record == nullptr) {
        SignalSema(__sce_eh_sema_id);
        return 0;
    }
    unsigned array = record->array;
    if (array == 0 || array == record->entries) {
        frame_init(record);
        array = record->array;
    }
    const unsigned entries = record->entries;
    unsigned result;
    if (array == 0) {
        result = search_fdes(entries, pc);
    } else if (array == entries) {
        const unsigned* list = at<unsigned>(array);
        do {
            result = search_fdes(*list, pc);
            ++list;
        } while (result == 0 && *list != 0);
    } else {
        SignalSema(__sce_eh_sema_id);
        unsigned low = 0;
        unsigned high = record->count;
        if (high == 0) return 0;
        const unsigned* table = at<unsigned>(record->array);
        while (low < high) {
            const unsigned middle = (low + high) >> 1;
            const unsigned fde = table[middle];
            const unsigned* words = at<unsigned>(fde);
            if (pc < words[2]) {
                high = middle;
            } else if (pc < words[2] + words[3]) {
                return fde;
            } else {
                low = middle + 1;
            }
        }
        return 0;
    }
    SignalSema(__sce_eh_sema_id);
    return result;
}
}

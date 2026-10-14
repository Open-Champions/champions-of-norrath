// Observed find_fde at 0x002fc8c8, 424 bytes (a local routine of the C++ exception runtime), pinned SLUS_205.65.
// It locates the frame-description entry that covers a code address among the registered object records, which are
// chained through a global list head `objects` (0x008eab18, tentative local storage). Record layout, 32-bit words:
//    0 first address covered     4 end address     8 entry list (word-sized length-prefixed entries)
//   12 sorted entry-pointer array (0 until initialised)     16 element count of that array     20 next record
// The whole routine runs under the kernel semaphore whose id is stored at __sce_eh_sema_id (0x00444a34):
// WaitSema first, SignalSema on every exit.
//   * The list is walked from the head. A record whose first address is 0 is initialised with frame_init
//     (0x002fc760) and reread. A record applies when first <= pc < end; none applying returns 0.
//   * If the array word is 0, or equals the entry-list word, the record is initialised with frame_init and both
//     words are reread.
//   * Array word 0: result of search_fdes (0x002fc708) on the entry list.
//   * Array word equal to the entry-list word: that word is then treated as a zero-terminated sequence of lists, each
//     searched in order with search_fdes; the first nonzero answer is the result, else 0.
//   * Otherwise the semaphore is released and the array (count may be 0, giving 0) is binary searched: with
//     lo = 0 and hi = count, mid = (lo + hi) >> 1; an entry whose start (word 2) is above pc moves hi to mid; one
//     with pc < start + length (word 3) is the result; otherwise lo = mid + 1.
// Inferred: the sort order is by start address; the record is a registered image's exception-table descriptor.
// Unknown: frame_init and search_fdes bodies, how records are registered. Nothing was executed on the EE.
extern "C" {
struct w15_fde_object {
    unsigned first;
    unsigned end;
    unsigned* entries;
    unsigned** array;
    unsigned count;
    w15_fde_object* next;
};

extern int __sce_eh_sema_id;
extern w15_fde_object* objects;
int WaitSema(int id);
int SignalSema(int id);
void frame_init(w15_fde_object* object);
unsigned* search_fdes(unsigned* entries, unsigned pc);

unsigned* find_fde(unsigned pc) {
    unsigned* result;
    WaitSema(__sce_eh_sema_id);
    w15_fde_object* object = objects;
    for (; object != nullptr; object = object->next) {
        if (object->first == 0) frame_init(object);
        if (pc >= object->first && pc < object->end) break;
    }
    if (object == nullptr) {
        SignalSema(__sce_eh_sema_id);
        return nullptr;
    }
    unsigned** array = object->array;
    if (array == nullptr || array == reinterpret_cast<unsigned**>(object->entries)) {
        frame_init(object);
        array = object->array;
    }
    unsigned* entries = object->entries;
    if (array == nullptr) {
        result = search_fdes(entries, pc);
    } else if (array == reinterpret_cast<unsigned**>(entries)) {
        unsigned** list = array;
        result = nullptr;
        do {
            result = search_fdes(*list, pc);
            ++list;
        } while (result == nullptr && *list != nullptr);
    } else {
        SignalSema(__sce_eh_sema_id);
        unsigned lo = 0, hi = object->count;
        if (hi == 0) return nullptr;
        array = object->array;
        while (lo < hi) {
            const unsigned mid = (lo + hi) >> 1;
            unsigned* fde = array[mid];
            if (pc < fde[2]) {
                hi = mid;
            } else if (pc < fde[2] + fde[3]) {
                return fde;
            } else {
                lo = mid + 1;
            }
        }
        return nullptr;
    }
    SignalSema(__sce_eh_sema_id);
    return result;
}
}

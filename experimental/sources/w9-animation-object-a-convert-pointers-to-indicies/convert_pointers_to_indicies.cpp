#include "../w9-animation-object-a-common/mem.hpp"
// Observed convertPointersToIndicies__Fi at 0x0024fcb0, 808 bytes. Argument p is a player
// number. Records are 23984 bytes: the live record is player[p] (0x00408600) and the
// destination record is tempPlayerStruct[p] (0x007e05e0). In order:
//   1. playerPrintPaperDoll(p) is called (it is a bare return in the image).
//   2. The whole 23984-byte record is copied from player[p] to tempPlayerStruct[p].
//   3. Nineteen pointer words, at record offsets 21896 through 21968 in steps of four
//      (sixteen from +21896, then +21960, +21964 and +21968), are each replaced in the copy by
//      an index. The index of a zero word is -1. Otherwise the three item lists of the
//      live record are searched: list n (n = 0..2) has its element count at +4936 + 4616*n and
//      its 72-byte elements from +328 + 4616*n; a word equal to the address of element i of
//      list n becomes n*1000 + i. A word that is in no list becomes -1.
//   4. For list n = 0, 1, 2 of the copy, the routine pack(item description) is called on each
//      of its elements in order (count read from the copy before each step).
// Inferred: this turns a player's item pointers into stable indices before saving, then
// packs the items; the lists are the three 64-slot item arrays seen elsewhere. No field
// names are claimed.
// Unknown: what the nineteen words are (they read as equipped-item pointers), why the first
// search uses the live record while the packing uses the copy, and whether the 1000 stride
// is a contract or a convenience.
extern "C" {
extern unsigned char player[];
extern unsigned char tempPlayerStruct[];
void playerPrintPaperDoll__Fi(int player_number);
void pack__4itemPQ24item11description(void* description);
}

namespace {
const unsigned record_size = 23984u;
const unsigned list_stride = 4616u;

// Looks a stored item address up in the live record's three lists.
int index_of(const unsigned char* live, unsigned word) {
    using namespace norrath_ao;
    if (word == 0u) return -1;
    for (int list = 0; list < 3; ++list) {
        const int count = static_cast<int>(load_word(live + 4936u + list_stride * list));
        for (int element = 0; element < count; ++element) {
            const unsigned char* candidate = live + 328u + list_stride * list + 72u * element;
#ifdef NORRATH_HOST_ARENA
            const unsigned address = norrath_host_address(candidate);
#else
            const unsigned address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(candidate));
#endif
            if (address == word) return element + 1000 * list;
        }
    }
    return -1;
}
}

extern "C" void convertPointersToIndicies__Fi(int player_number) {
    using namespace norrath_ao;
    playerPrintPaperDoll__Fi(player_number);
    unsigned char* live = player + record_size * player_number;
    unsigned char* copy = tempPlayerStruct + record_size * player_number;
    const unsigned long long* from = reinterpret_cast<const unsigned long long*>(live);
    unsigned long long* to = reinterpret_cast<unsigned long long*>(copy);
    for (unsigned i = 0; i < record_size / 8u; ++i) {
        to[i] = from[i];
        __asm__ volatile("" ::: "memory");
    }
    for (unsigned offset = 21896u; offset <= 21968u; offset += 4u) {
        store_word(copy + offset, static_cast<unsigned>(index_of(live, load_word(live + offset))));
    }
    for (int list = 0; list < 3; ++list) {
        const int count = static_cast<int>(load_word(copy + 4936u + list_stride * list));
        for (int element = 0; element < count; ++element) {
            pack__4itemPQ24item11description(copy + 328u + list_stride * list + 72u * element);
        }
    }
}

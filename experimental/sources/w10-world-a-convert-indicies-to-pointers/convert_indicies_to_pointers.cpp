// Observed convertIndiciesToPointers__Fi at 0x0024ffd8, 760 bytes in SLUS_205.65. The
// argument n is a player number; records are 23984 bytes in `player` (0x00408600).
// This is the inverse of the authored convertPointersToIndicies__Fi (same record offsets).
//  1. For list k = 0, 1, 2 (each list's element count is the signed word at +4936 + 4616*k,
//     read again before every step) unpack(element) is called on each 72-byte element starting
//     at +328 + 4616*k, in order. unpack turns the stored item row index into a row pointer.
//  2. Nineteen words at record offsets 21896 through 21968 in steps of four are replaced, in
//     ascending order. A word equal to -1 becomes zero. Any other value v is looked up in the
//     three lists: list k element i matches when k*1000 + i equals v, and the word becomes the
//     address of that element (record + 328 + 4616*k + 72*i). A value matching no element
//     (including one at or past a list's count) becomes zero.
//  3. playerPrintPaperDoll(n) is called last (the final word is stored in its call delay slot).
// Returns nothing. Inferred: after a load, saved indices are turned back into item pointers.
// Unknown: field names of the 19 words (they read as equipped-item references), and what
// playerPrintPaperDoll does here (the sibling source records it as a bare return).
extern "C" {
extern unsigned char player[];
void unpack__4itemPQ24item11description(void* description);
void playerPrintPaperDoll__Fi(int player_number);
}

namespace {
const unsigned record_size = 23984u;
const unsigned list_stride = 4616u;

inline int& count_of(unsigned char* record, int list) {
    return *reinterpret_cast<int*>(record + 4936u + list_stride * list);
}

inline unsigned pointer_for_index(unsigned char* record, int index) {
    if (index == -1) return 0u;
    for (int list = 0; list < 3; ++list) {
        const int count = count_of(record, list);
        for (int element = 0; element < count; ++element) {
            if (element + 1000 * list == index) {
                return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(
                    record + 328u + list_stride * list + 72u * element));
            }
        }
    }
    return 0u;
}
}

extern "C" void convertIndiciesToPointers__Fi(int n) {
    unsigned char* record = player + record_size * n;
    for (int list = 0; list < 3; ++list) {
        for (int element = 0; element < count_of(record, list); ++element) {
            unpack__4itemPQ24item11description(record + 328u + list_stride * list + 72u * element);
        }
    }
    for (unsigned offset = 21896u; offset <= 21968u; offset += 4u) {
        unsigned* word = reinterpret_cast<unsigned*>(record + offset);
        *word = pointer_for_index(record, static_cast<int>(*word));
    }
    playerPrintPaperDoll__Fi(n);
}

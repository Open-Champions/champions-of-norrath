#include "../level-anim-load-common/mem.hpp"
// Observed animLoadBoneMatricies__FP6PacketP14AnimationState at 0x001628e0, 268 bytes.
// state+0 holds the evaluated result record. When it equals the record emergencyResult
// (0x006da800) the word is cleared, the word at record+1552 is cleared and the record
// is dropped. With no record, evaluateAnimation__FP14AnimationStateb(state, 1) is called
// and state+0 reread. A record then gets the current drawFrameNm stored at +1564 and, when
// its word at +1560 is nonzero, it is unlinked from a doubly linked list (+1556 previous,
// +1560 next; a missing previous makes the next the new arListBack) and appended after
// the list end arList: next->prev = prev, record->prev = arList, arList->next = record,
// arList = record, record->next = 0. With no record at all identityResult is used instead.
// The packet pointer pair is then advanced: the word at packet+0 (the write cursor) moves
// by 32 bytes and the word at packet+4 becomes the old cursor + 16. Two 16-byte entries are
// written at the old cursor: first the 64-bit word (record << 32) | ((24 * halfword at
// record+8 + 31) >> 4, sign-extended to 64 bits) | 0x30000000 followed by a zero 64-bit word, then at +16 the 64-bit
// words 0x10000000 and 0x1400003a. Nothing is returned.
// Inferred: the first entry is a DMA reference tag (address in the upper half, quadword
// count of the bone matrices, ID field 3) and the second a count-zero tag carrying a
// VIF micro-program call code with immediate 0x3a; the list is a recency list of results.
// Unknown: what the emergency record is for, who reads packet+4, and arList's initialisation.
extern "C" {
extern unsigned char emergencyResult[];
extern unsigned char identityResult[];
extern int drawFrameNm;
extern unsigned char* arList;
extern unsigned char* arListBack;
void evaluateAnimation__FP14AnimationStateb(unsigned char* state, bool flag);
void animLoadBoneMatricies__FP6PacketP14AnimationState(unsigned char* packet, unsigned char* state) {
    using namespace norrath_lal;
    unsigned char* result = load_pointer(state);
    if (result == emergencyResult) {
        store_word(state, 0);
        store_word(result + 1552, 0);
        result = nullptr;
    }
    if (result == nullptr) {
        evaluateAnimation__FP14AnimationStateb(state, true);
        result = load_pointer(state);
    }
    if (result != nullptr) {
        store_word(result + 1564, static_cast<unsigned>(drawFrameNm));
        unsigned char* next = load_pointer(result + 1560);
        if (next != nullptr) {
            unsigned char* previous = load_pointer(result + 1556);
            if (previous == nullptr) arListBack = next;
            else store_pointer(previous + 1560, next);
            unsigned char* tail = arList;
            next = load_pointer(result + 1560);
            previous = load_pointer(result + 1556);
            store_pointer(next + 1556, previous);
            store_pointer(result + 1556, tail);
            store_pointer(tail + 1560, result);
            arList = result;
            store_word(result + 1560, 0);
        }
    } else {
        result = identityResult;
    }
    const int bones = load_signed_half(result + 8);
    const long long quads = static_cast<long long>(bones * 24 + 31) >> 4;
    unsigned char* cursor = load_pointer(packet);
    unsigned char* second = cursor + 16;
    store_pointer(packet, cursor + 32);
    store_pointer(packet + 4, second);
#ifdef NORRATH_HOST_ARENA
    const unsigned address = norrath_host_address(result);
#else
    const unsigned address = reinterpret_cast<__UINTPTR_TYPE__>(result);
#endif
    store_word(cursor + 8, 0);
    store_word(cursor + 12, 0);
    const unsigned long long tag = (static_cast<unsigned long long>(address) << 32) |
                                   static_cast<unsigned long long>(quads) | 0x30000000ull;
    store_word(cursor + 0, static_cast<unsigned>(tag));
    store_word(cursor + 4, static_cast<unsigned>(tag >> 32));
    store_word(second + 8, 0x1400003au);
    store_word(second + 12, 0);
    store_word(second + 0, 0x10000000u);
    store_word(second + 4, 0);
}
}

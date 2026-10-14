#include "../w9-animation-object-a-common/mem.hpp"
// Observed animMapAnimationSounds__FP15AnimationHeader at 0x0015fd98, 624 bytes. Header
// words used: +52 a table of 8-byte records ended by a record whose first word (a name
// pointer) is zero; +56 a table of 28-byte event records ended by a record whose first
// word is 0x7fffffff; +61 a byte; +64 an int result. In order:
//   1. A zero +52 returns at once.
//   2. Each name record gets its second word set to soundMapSound(name, 0); count is the
//      number of records. The +52 pointer is read again after every call.
//   3. For each event, with the signed halfword at event+4 (call it the index): when the
//      index is not negative and the name at that index equals "*bodyfall*" it becomes -3,
//      else when it equals "*blank*" it becomes -4 (the halfword and the +52/+56 pointers
//      are read again between the two compares).
//   4. When the byte at +61 is zero, bytes 10 through 14 of every event are cleared one
//      store at a time (the +56 pointer is read again before each store).
//   5. When the byte at +61 is below 2, each event's halfword at +4 is stored back, and if
//      it is below -2 or not below count it is replaced by 0 (count nonzero) or -1.
//   6. The int at +64 is set to (int)(soundComputeSequenceLength(header) * 60.0).
// Inferred: indexes -3 and -4 are markers for the two special sound names; -1 and 0 are
// the "no sound" fixups when an index is out of range. Names and field meanings are not
// claimed; "*bodyfall*" and "*blank*" are the literal strings the original compares against.
// Unknown: the layout of the event records beyond the word at +0, the halfword at +4 and
// the cleared bytes 10..14, and what the byte at +61 selects.
extern "C" {
int soundMapSound__FPci(char* name, int flag);
float soundComputeSequenceLength__FP15AnimationHeader(unsigned char* header);
int strcmp(const char* left, const char* right);
}

namespace {
const unsigned end_marker = 0x7fffffffu;
}

extern "C" void animMapAnimationSounds__FP15AnimationHeader(unsigned char* header) {
    using namespace norrath_ao;
    unsigned char* names = load_pointer(header + 52);
    if (names == nullptr) return;
    int count = 0;
    if (load_word(names) != 0u) {
        unsigned offset = 0;
        do {
            ++count;
            const int sound = soundMapSound__FPci(reinterpret_cast<char*>(load_pointer(names + offset)), 0);
            names = load_pointer(header + 52);
            store_word(names + offset + 4, static_cast<unsigned>(sound));
            offset += 8;
        } while (load_word(names + offset) != 0u);
    }
    unsigned char* events = load_pointer(header + 56);
    if (load_word(events) != end_marker) {
        unsigned offset = 0;
        do {
            const int index = load_signed_half(events + offset + 4);
            if (index >= 0) {
                if (strcmp(reinterpret_cast<const char*>(load_pointer(load_pointer(header + 52) + index * 8)), "*bodyfall*") == 0) {
                    events = load_pointer(header + 56);
                    store_half(events + offset + 4, static_cast<unsigned>(-3));
                } else {
                    const int again = load_signed_half(load_pointer(header + 56) + offset + 4);
                    if (strcmp(reinterpret_cast<const char*>(load_pointer(load_pointer(header + 52) + again * 8)), "*blank*") == 0) {
                        events = load_pointer(header + 56);
                        store_half(events + offset + 4, static_cast<unsigned>(-4));
                    } else {
                        events = load_pointer(header + 56);
                    }
                }
            }
            offset += 28;
        } while (load_word(events + offset) != end_marker);
    }
    const unsigned kind = header[61];
    if (kind == 0u) {
        if (load_word(events) != end_marker) {
            unsigned offset = 0;
            do {
                load_pointer(header + 56)[offset + 10] = 0;
                load_pointer(header + 56)[offset + 11] = 0;
                load_pointer(header + 56)[offset + 12] = 0;
                load_pointer(header + 56)[offset + 13] = 0;
                load_pointer(header + 56)[offset + 14] = 0;
                offset += 28;
                events = load_pointer(header + 56);
            } while (load_word(events + offset) != end_marker);
        }
    }
    if (header[61] < 2u) {
        if (load_word(events) != end_marker) {
            unsigned offset = 0;
            do {
                unsigned char* event = events + offset;
                const int index = load_signed_half(event + 4);
                store_half(event + 4, static_cast<unsigned>(index));
                offset += 28;
                if (index < -2 || !(index < count)) store_half(event + 4, count != 0 ? 0u : static_cast<unsigned>(-1));
                events = load_pointer(header + 56);
            } while (load_word(events + offset) != end_marker);
        }
    }
    const float length = soundComputeSequenceLength__FP15AnimationHeader(header);
    store_word(header + 64, static_cast<unsigned>(static_cast<int>(length * 60.0f)));
}

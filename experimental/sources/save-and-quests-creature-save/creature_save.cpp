#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_save__8CreaturePc at 0x001119d8, 436 bytes; the counterpart of
// msg_load__8CreaturePc. Fields keep their observed byte offsets; none is
// named. The buffer receives, in order:
//   word at object+244; the single at object+560 converted to an integer;
//   the string-pool index (objectSaveAddToStringPool) of the text whose
//   pointer is at object+892.
// When the single at object+560 is at most zero (a NaN skips this) one more
// word follows: object+36 points to a table of 8-byte entries (text pointer,
// then a header pointer). Starting at entry 1 and stopping at the first entry
// whose text pointer is zero, the entry whose header pointer equals the word
// at object + 264 + (signed byte at object+292) * 32 supplies its text, whose
// pool index is stored; no match, or an empty entry 1, stores -1.
// If bit 0 of the 64-bit flags at object+160 is set, the single at object+564
// converted to an integer, and the signed byte at object+76 as a word, follow.
// Then come the byte at object+570 and a mask byte (1 for flag bit 24, 2 for
// bit 32, 4 for bit 22, 8 for bit 23) and two zero bytes. The return is the
// number of bytes written. Conversions truncate toward zero.
extern "C" {
int objectSaveAddToStringPool__FPCc(const char* text);

int msg_save__8CreaturePc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* out = reinterpret_cast<unsigned char*>(buffer);
    unsigned char* start = out;
    write_word(out, read_word(object + 244));
    out += 4;
    write_word(out, static_cast<unsigned>(static_cast<int>(read_single(object + 560))));
    out += 4;
    write_word(out, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(
                        static_cast<const char*>(pointer(read_word(object + 892))))));
    out += 4;
    if (read_single(object + 560) <= 0.0f) {
        const unsigned char* table = static_cast<const unsigned char*>(pointer(read_word(object + 36)));
        const signed char selector = static_cast<signed char>(object[292]);
        const unsigned current = read_word(object + 264 + selector * 32);
        int result = -1;
        for (int index = 1;; ++index) {
            const unsigned char* entry = table + index * 8;
            const unsigned text = read_word(entry);
            if (text == 0) break;
            if (read_word(entry + 4) == current) {
                result = objectSaveAddToStringPool__FPCc(static_cast<const char*>(pointer(text)));
                break;
            }
        }
        write_word(out, static_cast<unsigned>(result));
        out += 4;
    }
    const unsigned long long flags = read_u64(object + 160);
    if ((flags & 1u) != 0u) {
        write_word(out, static_cast<unsigned>(static_cast<int>(read_single(object + 564))));
        out += 4;
        write_word(out, static_cast<unsigned>(static_cast<int>(static_cast<signed char>(object[76]))));
        out += 4;
    }
    *out++ = object[570];
    unsigned mask = 0;
    if ((flags & 0x01000000ull) != 0) mask |= 1u;
    if ((flags & 0x100000000ull) != 0) mask |= 2u;
    if ((flags & 0x400000ull) != 0) mask |= 4u;
    if ((flags & 0x800000ull) != 0) mask |= 8u;
    *out++ = static_cast<unsigned char>(mask);
    *out++ = 0;
    *out++ = 0;
    return static_cast<int>(out - start);
}
}

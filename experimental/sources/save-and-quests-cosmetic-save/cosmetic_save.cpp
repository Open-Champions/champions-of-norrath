#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_save__16CosmeticPropAnimPc at 0x00155200, 200 bytes; the
// counterpart of msg_load__16CosmeticPropAnimPc. Fields keep their observed
// byte offsets and are not named. The buffer receives four string-pool indexes
// (objectSaveAddToStringPool) of the text pointers at object+76, +80, +84 and
// +88, then the words at object+12, +148 and +144, then the singles at
// object+32 and object+28 as integers truncated toward zero, each stored as a
// halfword. The return is the byte count, 32.
extern "C" {
int objectSaveAddToStringPool__FPCc(const char* text);

int msg_save__16CosmeticPropAnimPc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* out = reinterpret_cast<unsigned char*>(buffer);
    for (unsigned offset = 76; offset <= 88; offset += 4) {
        write_word(out, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(
                            static_cast<const char*>(pointer(read_word(object + offset))))));
        out += 4;
    }
    const int first = static_cast<int>(read_single(object + 32));
    const int second = static_cast<int>(read_single(object + 28));
    write_word(out, read_word(object + 12));
    write_word(out + 4, read_word(object + 148));
    write_word(out + 8, read_word(object + 144));
    out += 12;
    out[0] = static_cast<unsigned char>(first);
    out[1] = static_cast<unsigned char>(first >> 8);
    out[2] = static_cast<unsigned char>(second);
    out[3] = static_cast<unsigned char>(second >> 8);
    return static_cast<int>(out + 4 - reinterpret_cast<unsigned char*>(buffer));
}
}

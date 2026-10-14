#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_load__16CosmeticPropAnimPc at 0x0014b1c0, 356 bytes; it reads
// what msg_save__16CosmeticPropAnimPc wrote. Fields keep their observed byte
// offsets and are not named. In order: four pool indexes
// (objectSaveGetFromStringPool) stored as pointers at object+76, +80, +84 and
// +88; words to object+12, object+148 and object+144; two signed halfwords.
// The first becomes a single at object+32 and its low byte goes to object+41;
// the second becomes a single at object+28 and its low byte goes to object+40.
// lumpFindResource(object+76, object+80) goes to object+92 and
// lumpFindResource(object+76, object+84) to object+96. The text at object+88
// is then looked up the same way and added with animAdd to the animation state
// at object+100, with count 4 if bit 1 of the word at object+148 is set, else
// 1. When bit 0 of that word is set the state is set to its end. Finally the
// object goes onto the slow run list. The return is the byte count, 32.
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int index);
void* lumpFindResource__FPcT0(char* first, char* second);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int count);
void animSetToEnd__FP14AnimationState(void* state);
void objectAddToSlowRunList__FP10GameObject(void* object);

int msg_load__16CosmeticPropAnimPc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* in = reinterpret_cast<const unsigned char*>(buffer);
    for (unsigned offset = 76; offset <= 88; offset += 4) {
        write_word(object + offset, pointer_bits(objectSaveGetFromStringPool__Fi(signed_word(read_word(in)))));
        in += 4;
    }
    write_word(object + 12, read_word(in));
    write_word(object + 148, read_word(in + 4));
    write_word(object + 144, read_word(in + 8));
    in += 12;
    const int first = read_i16(in);
    const int second = read_i16(in + 2);
    in += 4;
    write_single(object + 32, static_cast<float>(first));
    write_single(object + 28, static_cast<float>(second));
    object[41] = static_cast<unsigned char>(first);
    object[40] = static_cast<unsigned char>(second);
    char* group = static_cast<char*>(pointer(read_word(object + 76)));
    write_word(object + 92, pointer_bits(lumpFindResource__FPcT0(group, static_cast<char*>(pointer(read_word(object + 80))))));
    write_word(object + 96, pointer_bits(lumpFindResource__FPcT0(
                                static_cast<char*>(pointer(read_word(object + 76))),
                                static_cast<char*>(pointer(read_word(object + 84))))));
    const int count = (read_word(object + 148) & 2u) != 0u ? 4 : 1;
    void* header = lumpFindResource__FPcT0(static_cast<char*>(pointer(read_word(object + 76))),
                                           static_cast<char*>(pointer(read_word(object + 88))));
    animAdd__FP14AnimationStateP15AnimationHeaderi(object + 100, header, count);
    if ((read_word(object + 148) & 1u) != 0u) animSetToEnd__FP14AnimationState(object + 100);
    objectAddToSlowRunList__FP10GameObject(self);
    return static_cast<int>(in - reinterpret_cast<const unsigned char*>(buffer));
}
}

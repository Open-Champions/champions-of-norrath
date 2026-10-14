#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_load__4PropPc at 0x0014b808, 472 bytes; it reads what
// msg_save__4PropPc wrote. Fields keep their observed byte offsets and are not
// named. In order:
//   three pool indexes -> objectSaveGetFromStringPool; the results are copied
//   with strcpy to the text arrays at object+84, +100 and +164 (all three
//   lookups happen before the first copy);
//   bit 12 (0x1000) of the word at object+12 is set;
//   a signed byte: -1 means eight value bytes follow after seven skipped
//   bytes (word -1 occupies four, then the value, then one zero byte), else
//   the 64-bit value at object+232 is 1 << byte (shift count taken modulo 64);
//   three signed bytes times 0.007874016 (bit pattern 0x3c010204) and one
//   signed halfword times 0.0001 (0x38d1b717) form a quaternion, turned into
//   the matrix at object+272 by quatToMat;
//   three signed halfwords stored as singles at object+308, +312 and +316
//   (no scaling);
//   the pointer results of lumpFindResource(object+84, object+100) and
//   lumpFindResource(object+84, object+164) go to object+76 and object+80.
// The return is the number of bytes consumed.
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int index);
char* strcpy(char* destination, const char* source);
void quatToMat__FP8Matrix34P4Quat(void* matrix, void* quaternion);
void* lumpFindResource__FPcT0(char* first, char* second);

int msg_load__4PropPc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* start = reinterpret_cast<const unsigned char*>(buffer);
    const unsigned char* in = start;
    const char* first = objectSaveGetFromStringPool__Fi(signed_word(read_word(in)));
    const char* second = objectSaveGetFromStringPool__Fi(signed_word(read_word(in + 4)));
    const char* third = objectSaveGetFromStringPool__Fi(signed_word(read_word(in + 8)));
    in += 12;
    strcpy(reinterpret_cast<char*>(object + 84), first);
    strcpy(reinterpret_cast<char*>(object + 100), second);
    strcpy(reinterpret_cast<char*>(object + 164), third);
    write_word(object + 12, read_word(object + 12) | 0x1000u);
    const signed char selector = static_cast<signed char>(in[0]);
    in += 1;
    unsigned long long value;
    if (selector == -1) {
        in += 3;
        value = read_u64(in);
        in += 8;
        in += 1;
    } else {
        value = 1ull << (selector & 63);
    }
    write_word(object + 232, static_cast<unsigned>(value));
    write_word(object + 236, static_cast<unsigned>(value >> 32));
    const float byte_scale = __builtin_bit_cast(float, 0x3c010204u);
    const float half_scale = __builtin_bit_cast(float, 0x38d1b717u);
    float quaternion[4];
    quaternion[0] = static_cast<float>(static_cast<signed char>(in[0])) * byte_scale;
    quaternion[1] = static_cast<float>(static_cast<signed char>(in[1])) * byte_scale;
    quaternion[2] = static_cast<float>(static_cast<signed char>(in[2])) * byte_scale;
    in += 3;
    quaternion[3] = static_cast<float>(read_i16(in)) * half_scale;
    in += 2;
    quatToMat__FP8Matrix34P4Quat(object + 272, quaternion);
    for (unsigned offset = 308; offset <= 316; offset += 4) {
        write_single(object + offset, static_cast<float>(read_i16(in)));
        in += 2;
    }
    write_word(object + 76, pointer_bits(lumpFindResource__FPcT0(reinterpret_cast<char*>(object + 84), reinterpret_cast<char*>(object + 100))));
    write_word(object + 80, pointer_bits(lumpFindResource__FPcT0(reinterpret_cast<char*>(object + 84), reinterpret_cast<char*>(object + 164))));
    return static_cast<int>(in - start);
}
}

#include "../save-and-quests-bytes/bytes.hpp"
// Observed msg_save__4PropPc at 0x0014b690, 372 bytes; the counterpart of
// msg_load__4PropPc. Fields keep their observed byte offsets and are not
// named. The buffer receives, in order:
//   three string-pool indexes (objectSaveAddToStringPool) of the texts that
//   start at object+84, object+100 and object+164;
//   the 64-bit value at object+232: if it equals 1 << lilog2(value) one byte,
//   lilog2(value), follows; otherwise a word of -1, the eight value bytes and
//   one zero byte follow;
//   the matrix at object+272 turned into a quaternion (matToQuaternion), then
//   normalised (quatNormalize); its first three components times 127 as one
//   byte each and its fourth times 10000 as a halfword;
//   the singles at object+308, +312 and +316 as halfwords.
// Conversions truncate toward zero and keep the low bits. The return is the
// number of bytes written.
extern "C" {
int objectSaveAddToStringPool__FPCc(const char* text);
int lilog2__Fl(long long value);
void matToQuaternion__FP4QuatP8Matrix34(void* quaternion, void* matrix);
void quatNormalize__FP4Quat(void* quaternion);

int msg_save__4PropPc(void* self, char* buffer) {
    using namespace sq;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* out = reinterpret_cast<unsigned char*>(buffer);
    unsigned char* start = out;
    write_word(out, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(object + 84))));
    out += 4;
    write_word(out, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(object + 100))));
    out += 4;
    write_word(out, static_cast<unsigned>(objectSaveAddToStringPool__FPCc(reinterpret_cast<const char*>(object + 164))));
    out += 4;
    const unsigned long long value = read_u64(object + 232);
    const int exponent = lilog2__Fl(static_cast<long long>(value));
    if ((1ull << (exponent & 63)) == value) {
        *out = static_cast<unsigned char>(lilog2__Fl(static_cast<long long>(value)));
    } else {
        write_word(out, 0xffffffffu);
        out += 4;
        write_word(out, static_cast<unsigned>(value));
        write_word(out + 4, static_cast<unsigned>(value >> 32));
        out += 8;
        *out = 0;
    }
    out += 1;
    float quaternion[4];
    matToQuaternion__FP4QuatP8Matrix34(quaternion, object + 272);
    quatNormalize__FP4Quat(quaternion);
    const float scale = 127.0f;
    const int q0 = static_cast<int>(quaternion[0] * scale);
    const int q1 = static_cast<int>(quaternion[1] * scale);
    const int q2 = static_cast<int>(quaternion[2] * scale);
    const int q3 = static_cast<int>(quaternion[3] * 10000.0f);
    out[0] = static_cast<unsigned char>(q0);
    out[1] = static_cast<unsigned char>(q1);
    out[2] = static_cast<unsigned char>(q2);
    out += 3;
    out[0] = static_cast<unsigned char>(q3);
    out[1] = static_cast<unsigned char>(q3 >> 8);
    out += 2;
    for (unsigned offset = 308; offset <= 316; offset += 4) {
        const int v = static_cast<int>(read_single(object + offset));
        out[0] = static_cast<unsigned char>(v);
        out[1] = static_cast<unsigned char>(v >> 8);
        out += 2;
    }
    return static_cast<int>(out - start);
}
}

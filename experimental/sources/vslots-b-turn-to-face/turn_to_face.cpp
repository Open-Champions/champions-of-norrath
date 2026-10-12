#include "../vslots-b-bytes/bytes.hpp"
// Observed turnToFace__8CreatureiG6Point3 at 0x10a2a8, 380 bytes.
// iatan2 receives point.y-self.y and point.x-self.x. Flag 1 adds 32768 to
// that result before the unsigned half at object+56 is subtracted. The
// difference, the half at object+284, and the single at object+168 pass
// through cvt.w.s with the four words at 0x47fb90. The half at object+284 is
// then kept only inside the negated and positive limits. Byte object+800 in
// 1..60 scales that half; any other byte adds it as an unsigned half. The
// sum is stored at object+56. iatan2 is not defined here.
extern "C" int iatan2__Fff(float first, float second);
namespace {
__attribute__((always_inline)) inline float from_bits(unsigned bits) {
    float value;
    unsigned char* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
__attribute__((always_inline)) inline int cvt_w_s(float value) {
    unsigned raw;
    const unsigned char* in = reinterpret_cast<const unsigned char*>(&value);
    raw = unsigned(in[0]) | (unsigned(in[1]) << 8) | (unsigned(in[2]) << 16) | (unsigned(in[3]) << 24);
    const int sign = static_cast<int>((raw >> 31) & 1u);
    const unsigned exponent_field = (raw >> 23) & 0xffu;
    const unsigned fraction = (raw & 0x7fffffu) | (exponent_field == 0u ? 0u : 0x800000u);
    if (exponent_field == 255u) return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
    if (exponent_field == 0u) return 0;
    const int exponent = static_cast<int>(exponent_field) - 127;
    if (exponent >= 31) return sign != 0 ? static_cast<int>(0x80000000u) : 0x7fffffff;
    if (exponent < 0) {
        if (exponent == -1 && fraction == 0x800000u) return 0;
        if (exponent < -1) return 0;
        return sign != 0 ? -1 : 1;
    }
    const int shift = exponent - 23;
    unsigned magnitude;
    if (shift >= 0) {
        magnitude = fraction << shift;
    } else {
        const int places = -shift;
        unsigned base = fraction >> places;
        const unsigned remainder = fraction & ((1u << places) - 1u);
        const unsigned half = 1u << (places - 1);
        if (remainder > half || (remainder == half && (base & 1u) != 0u)) base += 1u;
        magnitude = base;
    }
    return sign != 0 ? -static_cast<int>(magnitude) : static_cast<int>(magnitude);
}
__attribute__((always_inline)) inline int low_half(unsigned value) {
    const unsigned low = value & 0xffffu;
    return low < 0x8000u ? static_cast<int>(low) : static_cast<int>(low) - 65536;
}
}
extern "C" void turnToFace__8CreatureiG6Point3(void* self, int flag, const void* point) {
    using namespace vslots_b;
    const Address base = address(self);
    const Address target = address(point);
    const float dx = read_single(target) - read_single(base + 16u);
    const float dy = read_single(target + 4u) - read_single(base + 20u);
    const int limit = cvt_w_s(read_single(base + 168u) * from_bits(0x44888888u));
    unsigned chosen = static_cast<unsigned>(iatan2__Fff(dy, dx));
    if ((flag ^ 1) == 0) chosen += 0x8000u;
    const unsigned facing = read_u16(base + 56u);
    const int delta = low_half(chosen - facing);
    const int step = cvt_w_s(static_cast<float>(delta) * from_bits(0x3d4cccccu));
    int sum = low_half(read_u16(base + 284u) + static_cast<unsigned>(step));
    const int decay = cvt_w_s(__builtin_sqrtf(from_bits(0x3e4cccccu)) * static_cast<float>(sum));
    sum = low_half(static_cast<unsigned>(sum) - static_cast<unsigned>(decay));
    const int negative = static_cast<int>(0u - static_cast<unsigned>(limit));
    int stored = sum;
    if (limit < sum) stored = limit;
    else if (sum < negative) stored = negative;
    write_u16(base + 284u, static_cast<unsigned>(stored));
    const unsigned speed = read_u8(base + 800u);
    const int signed_speed = speed < 0x80u ? static_cast<int>(speed) : static_cast<int>(speed) - 256;
    unsigned next;
    if ((speed - 1u) < 60u) {
        const int factor = 60 - signed_speed;
        const int half = read_i16(base + 284u);
        const int add = cvt_w_s(static_cast<float>(half) * (static_cast<float>(factor) * from_bits(0x3c888888u)));
        next = facing + static_cast<unsigned>(add);
    } else {
        next = facing + read_u16(base + 284u);
    }
    write_u16(base + 56u, next);
}

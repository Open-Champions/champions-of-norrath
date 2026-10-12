// Observed worldMoveDoor__FP12_worldHeaderiG6Point3s at 0x001757d8, 644 bytes.
// Arguments: world header, door index, a Point3 passed by address (three
// floats; the 12-byte struct is copied and each component is multiplied by
// 4.0f first), and a signed 16-bit angle. A negative door index does nothing.
// s = isin(angle) and c = icos(angle) are both float results. The door
// record is at (word at header+84) + index*28. Its signed halves at +20 and
// +22 (cx, cy) are rotated:
//   a = (c*cx*K - s*cy*K) * 32767.0f + 0.5f
//   b = (s*cx*K + c*cy*K) * 32767.0f + 0.5f
// with K = 1/32767 (float bits 0x38000100). floorf of each is converted to
// int and clamped to [-32768, 32767] (a -> A, b -> B). Then, for each of
// the signed half at door+4 records (index word(door+0)+k, 60 bytes each at
// the address in header+36), with px/py/pz = float(word at door+8/+12/+16)
// plus the scaled point:
//   flag word at rec+36 bit 0x40 set:   rec+44 = int(px*s + py*c)
//                                        rec+40 = int(px*c - py*s)
//   flag clear:                          rec+40 = int(px*c + py*s)
//                                        rec+44 = int(-px*s + py*c)
// both store half B at rec+56; then rec+48 = int(pz) and half A at rec+54.
// The door pointer and count are re-read after every record. Nothing is
// returned. The two constants live in the executable's read-only data.
// isin/icos are authored by another package; floorf is the external Newlib
// input. Record meanings beyond these offsets are not established.
#if defined(NORRATH_HOST_WORLD)
extern unsigned char* norrath_world_arena;
#endif

extern "C" {
float isin__Fi(int angle);
float icos__Fi(int angle);
float floorf(float value);
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline int half(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const short*>(base + offset);
}
inline unsigned char* table(const unsigned char* header, unsigned offset) {
    const unsigned bits = word(header, offset);
#if defined(NORRATH_HOST_WORLD)
    return norrath_world_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline int clamp_half(int value) {
    if (value > 32767) value = 32767;
    if (value < -32768) value = -32768;
    return value;
}
}

extern "C" void worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int door_index, const float* point, short angle) {
    unsigned char* header = static_cast<unsigned char*>(world);
    float scaled[3];
    scaled[0] = point[0] * 4.0f;
    scaled[1] = point[1] * 4.0f;
    scaled[2] = point[2] * 4.0f;
    if (door_index < 0) return;

    const float range = 32767.0f;
    const float inverse = 3.0518509447574615e-05f;
    const float s = isin__Fi(angle);
    const float c = icos__Fi(angle);
    const unsigned door_offset = static_cast<unsigned>(door_index) * 28u;
    const unsigned char* door = table(header, 84) + door_offset;
    const float cx = static_cast<float>(half(door, 20));
    const float cy = static_cast<float>(half(door, 22));
    const float a = ((c * cx) * inverse - (s * cy) * inverse) * range;
    const float b = ((s * cx) * inverse + (c * cy) * inverse) * range;
    const int rotated_a = clamp_half(static_cast<int>(floorf(a + 0.5f)));
    const int rotated_b = clamp_half(static_cast<int>(floorf(b + 0.5f)));

    for (int k = 0; k < half(table(header, 84) + door_offset, 4); ++k) {
        door = table(header, 84) + door_offset;
        const unsigned index = word(door, 0) + static_cast<unsigned>(k);
        unsigned char* record = table(header, 36) + index * 60u;
        const float px = static_cast<float>(static_cast<int>(word(door, 8))) + scaled[0];
        const float py = static_cast<float>(static_cast<int>(word(door, 12))) + scaled[1];
        const float pz = static_cast<float>(static_cast<int>(word(door, 16))) + scaled[2];
        int* out = reinterpret_cast<int*>(record);
        if ((word(record, 36) & 0x40u) != 0u) {
            out[11] = static_cast<int>(px * s + py * c);
            out[10] = static_cast<int>(px * c - py * s);
        } else {
            out[10] = static_cast<int>(px * c + py * s);
            out[11] = static_cast<int>(-px * s + py * c);
        }
        *reinterpret_cast<short*>(record + 56) = static_cast<short>(rotated_b);
        out[12] = static_cast<int>(pz);
        *reinterpret_cast<short*>(record + 54) = static_cast<short>(rotated_a);
    }
}

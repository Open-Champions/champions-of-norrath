// Observed lightSetupForWorld__FP6PacketP13PolysetHeader at 0x0013e230, extent 1160.
// Arguments: a Packet (its first word is the output cursor) and a polyset header.
// Returns an int: 0 when no light was used, otherwise 1.
//
// Header words used (byte offsets): floats at 8, 12, 16 and 20, 24, 28 are the two corners
// of a box; the word at 36 has bit 0 tested; the word at 52 has bits 16, 17 and 18 tested;
// halfwords at 54 and 56 are read only when bit 0 of the word at 36 is clear.
// Light words used: halfwords at 18, 20, 22, 24; floats at 36, 40, 44, 48, 52, 56.
//
// Steps:
//  1. findLightsInBox_forWorld(box copy, lights, 32) fills a 32-entry list of Light pointers
//     from the box at header+8 (six floats); the entry lights[31] is then set to null.
//  2. If lights[0] is not null: the box center is (corner + corner) * 0.5f per axis. For each
//     light up to the first null, the squared distance from the center to the position at
//     light+48 is (dx*dx + dy*dy) + dz*dz, truncated to int; key = (int << 7) | list index.
//     Four keys start at 0x1000001f and keep the four smallest (signed) by insertion.
//     The four chosen lights are lights[key & 31], nearest first; an unused key picks
//     lights[31], null.
//  3. The cursor gets 0x6d048000, then from base = cursor + 4: halfword quads at base + 8 *
//     slot, floats at base + 40 + 4 * slot (six rows of the slot, row stride 16), tags
//     0x1400000e at base+32, 0x6c06c000 at base+36, 0x14000004 at base+136. The cursor
//     becomes base + 140.
//  4. Chosen lights are taken in order until a null. A light whose float at +44 is at least
//     1.0f takes the highest free slot (starting at 3, going down), otherwise the lowest
//     (starting at 0, going up). For slot s: the four halfwords at light+18..24 go to the
//     quad; row 0 = the float at +44; rows 1 and 2 depend on bit 0 of header word 36:
//       set:   with i = (word52 >> 18) & 1 and p the position floats at +48:
//              a = p[i] * 0.25f, b = p[i ^ 1] * 0.25f; row 1 = (bit 16 of word52) ? -a : a;
//              row 2 = (bit 17 of word52) ? -b : b.
//       clear: u = (float)(short at 54) * k, v = (float)(short at 56) * k with k the float
//              1/32767 (0x38000100, a literal at 0x0047fcc4); row 1 = (u * px + v * py) *
//              0.25f; row 2 = ((-v) * px + u * py) * 0.25f, px and py at +48 and +52.
//     Row 3 = (float at +56) * 0.25f, row 4 = (float at +36 squared) * 0.0625f, row 5 =
//     -(float at +40) * 16.0f.
//  5. Every slot between the lowest unused and the highest unused (inclusive) gets zero
//     halfwords, row 0 = 1.0f and rows 1 to 5 zero.
// The result is 1 unless no light was placed in either end (low count 0 and high still 3).
// Inferred: this builds the world-lighting packet for the four nearest lights; a light
// with intensity at least 1 is placed from the other end of the same four slots.
// Unknown: what the rows mean to the consumer, and the meaning of the header flags.
struct Box {
    float v[6];
};

struct WorldLightPacket {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
void findLightsInBox_forWorld__FG3BoxPP5Lighti(Box, const unsigned char**, int);
}

namespace {
inline float fl(const unsigned char* base, int off) { return *reinterpret_cast<const float*>(base + off); }
inline unsigned wd(const unsigned char* base, int off) { return *reinterpret_cast<const unsigned*>(base + off); }
inline unsigned short hw(const unsigned char* base, int off) { return *reinterpret_cast<const unsigned short*>(base + off); }
inline short shw(const unsigned char* base, int off) { return *reinterpret_cast<const short*>(base + off); }

// Keeps the four smallest keys in order, as the original's chain of compare and select does.
inline void keep_smaller(int& slot, int& carry) {
    if (!(slot < carry)) {
        const int t = slot;
        slot = carry;
        carry = t;
    }
}
inline void insert_key(int& a, int& b, int& c, int& d, int key) {
    int carry = key;
    keep_smaller(a, carry);
    keep_smaller(b, carry);
    keep_smaller(c, carry);
    keep_smaller(d, carry);
}
}

extern "C" int lightSetupForWorld__FP6PacketP13PolysetHeader(WorldLightPacket* packet, const unsigned char* header) {
    const unsigned char* lights[32];
    Box box = *reinterpret_cast<const Box*>(header + 8);
    findLightsInBox_forWorld__FG3BoxPP5Lighti(box, lights, 32);
    lights[31] = nullptr;

    int k0 = 0x1000001f, k1 = k0, k2 = k0, k3 = k0;
    if (lights[0] != nullptr) {
        const float cx = (fl(header, 8) + fl(header, 20)) * 0.5f;
        const float cy = (fl(header, 12) + fl(header, 24)) * 0.5f;
        const float cz = (fl(header, 16) + fl(header, 28)) * 0.5f;
        int index = 0;
        while (true) {
            const unsigned char* light = lights[index];
            const unsigned char* next = lights[index + 1];
            const float dx = cx - fl(light, 48);
            const float dy = cy - fl(light, 52);
            const float dz = cz - fl(light, 56);
            const float distance = (dx * dx + dy * dy) + dz * dz;
            const int key = static_cast<int>(static_cast<unsigned>(static_cast<int>(distance)) << 7) | index;
            ++index;
            insert_key(k0, k1, k2, k3, key);
            if (next == nullptr) break;
        }
    }
    const unsigned char* chosen[4] = {lights[k0 & 0x1f], lights[k1 & 0x1f], lights[k2 & 0x1f], lights[k3 & 0x1f]};

    unsigned char* cursor = packet->cursor;
    *reinterpret_cast<unsigned*>(cursor) = 0x6d048000u;
    unsigned char* base = cursor + 4;
    packet->cursor = base + 140;
    *reinterpret_cast<unsigned*>(base + 32) = 0x1400000eu;
    *reinterpret_cast<unsigned*>(base + 36) = 0x6c06c000u;
    *reinterpret_cast<unsigned*>(base + 136) = 0x14000004u;
    unsigned char* rows = base + 40;

    int low = 0;
    int high = 3;
    for (int n = 0; n < 4 && chosen[n] != nullptr; ++n) {
        const unsigned char* light = chosen[n];
        const unsigned flags = wd(header, 36);
        const float intensity = fl(light, 44);
        int slot;
        if (1.0f <= intensity) {
            slot = high;
            --high;
        } else {
            slot = low;
            ++low;
        }
        unsigned short* quad = reinterpret_cast<unsigned short*>(base + slot * 8);
        float* row = reinterpret_cast<float*>(rows + slot * 4);
        quad[0] = hw(light, 18);
        row[0] = intensity;
        quad[1] = hw(light, 20);
        quad[2] = hw(light, 22);
        quad[3] = hw(light, 24);
        if ((flags & 1u) != 0u) {
            const unsigned control = wd(header, 52);
            const unsigned axis = (control >> 18) & 1u;
            const float* position = reinterpret_cast<const float*>(light + 48);
            float a = position[axis] * 0.25f;
            const float b = position[axis ^ 1u] * 0.25f;
            if ((control & 0x10000u) != 0u) a = -a;
            row[4] = a;
            row[8] = (wd(header, 52) & 0x20000u) != 0u ? -b : b;
        } else {
            const float k = 1.0f / 32767.0f;
            const float u = static_cast<float>(shw(header, 54)) * k;
            float v = static_cast<float>(shw(header, 56)) * k;
            const float s = u * fl(light, 48);
            const float t = v * fl(light, 52);
            v = -v;
            row[4] = (s + t) * 0.25f;
            const float p = v * fl(light, 48);
            const float q = u * fl(light, 52);
            row[8] = (p + q) * 0.25f;
        }
        row[12] = fl(light, 56) * 0.25f;
        const float squared = fl(light, 36) * fl(light, 36);
        row[16] = squared * 0.0625f;
        row[20] = -fl(light, 40) * 16.0f;
    }
    if (!(high < low)) {
        for (int slot = low; slot <= high; ++slot) {
            unsigned short* quad = reinterpret_cast<unsigned short*>(base + slot * 8);
            float* row = reinterpret_cast<float*>(rows + slot * 4);
            quad[0] = 0;
            quad[1] = 0;
            quad[2] = 0;
            quad[3] = 0;
            row[0] = 1.0f;
            row[4] = 0;
            row[8] = 0;
            row[12] = 0;
            row[16] = 0;
            row[20] = 0;
        }
    }
    return low != 0 || high != 3;
}

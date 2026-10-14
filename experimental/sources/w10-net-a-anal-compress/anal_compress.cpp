// Observed analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci
// at 0x00133a90, 1228 bytes. The first argument (the CCompress object) is not
// read. The second is a bit writer: a pointer to a halfword buffer and a signed
// bit position. The third is a state of two words: a step index and a running
// predicted value. The fourth is count unsigned bytes and the fifth is count.
// A scratch array of count bytes is placed on the stack. Three passes:
//  1. For each input byte: delta = (byte - predicted) / step_table[index]
//     (signed division, truncating); the sign is taken from delta before the
//     magnitude is limited, and a magnitude of 2 or more becomes 1. The scratch
//     byte is magnitude | (sign << 1). predicted moves by magnitude * step
//     (minus when the sign is set, floor 0; plus otherwise, ceiling 255) and
//     index moves by step_change[magnitude] limited to 0..8. Both are written
//     back to the state on every byte.
//  2. A run-length pass over bit 0 of the scratch bytes. Outside a group, zero
//     bits are counted; reaching 31 writes the 5-bit value 31 and restarts the
//     count. A set bit writes the 5-bit zero count and starts a group. Inside a
//     group every bit is written as one bit; after the sixth group element
//     (the starting bit counts as the first) a group that saw a set bit in its
//     written bits restarts, otherwise it ends. A nonzero zero count left at the
//     end is written as 5 bits.
//  3. A sign pass over the scratch bytes with bit 0 set: the first one writes
//     its sign bit as one bit. Each later one that changes sign first writes the
//     current run length as 3 bits; a run reaching 7 writes 7 as 3 bits and
//     restarts. A nonzero run left at the end is written as 3 bits.
// Bit writing ORs the new bits into the current halfword and stores the spill
// into the next halfword (overwriting it).
// Inferred: this compresses analog-stick bytes with a two-bit delta code; the
// state is carried between calls by the caller.
// Unknown: the CCompress and state type names beyond the mangled name, what a
// negative count does in the original (it adjusts the stack by a huge amount),
// and the format of the decoder beyond this pass structure.
namespace {
struct BitWriter {
    unsigned short* data;
    int position;
};

struct AnalState {
    int index;
    int value;
};

const short kStepTable[9] = {4, 6, 8, 12, 16, 24, 32, 48, 64};
const short kStepChange[2] = {-1, 2};

// Writes the low `width` bits of value at the writer position.
inline void put(BitWriter* writer, unsigned value, int width) {
    unsigned window = (value & ((1u << width) - 1u)) << (16 - width);
    int position = writer->position;
    unsigned short* unit = writer->data + (position >> 4);
    int shift = position & 15;
    unsigned short old = unit[0];
    writer->position = position + width;
    unit[0] = static_cast<unsigned short>(old | (window >> shift));
    unit[1] = static_cast<unsigned short>(window << (16 - shift));
}
}

extern "C" void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(
    void* compression, void* writer_argument, void* state_argument, unsigned char* bytes, int count) {
    BitWriter* writer = static_cast<BitWriter*>(writer_argument);
    AnalState* state = static_cast<AnalState*>(state_argument);
    unsigned char codes[count > 0 ? count : 1];

    for (int i = 0; i < count; ++i) {
        int index = state->index;
        int value = state->value;
        int step = kStepTable[index];
        int delta = (bytes[i] - value) / step;
        int sign = 0;
        if (delta < 0) {
            delta = -delta;
            sign = 1;
        }
        if (delta >= 2) delta = 1;
        int moved;
        if (sign) {
            moved = value - delta * step;
            state->value = moved < 0 ? 0 : moved;
        } else {
            moved = value + delta * step;
            state->value = moved > 255 ? 255 : moved;
        }
        int next = index + kStepChange[delta];
        if (next < 0) next = 0;
        state->index = next > 8 ? 8 : next;
        codes[i] = static_cast<unsigned char>(delta | (sign << 1));
    }

    int group = 0;
    int group_seen = 0;
    int zeros = 0;
    for (int i = 0; i < count; ++i) {
        int bit = codes[i] & 1;
        if (group != 0) {
            put(writer, bit, 1);
            group = group + 1;
            if (bit != 0) group_seen = 1;
            if (group == 6) {
                if (group_seen != 0) {
                    group = 1;
                    group_seen = 0;
                } else {
                    group = 0;
                }
            }
        } else if (bit != 0) {
            put(writer, zeros, 5);
            zeros = 0;
            group = 1;
            group_seen = 0;
        } else {
            zeros = zeros + 1;
            if (zeros == 31) {
                put(writer, 31, 5);
                zeros = 0;
            }
        }
    }
    if (zeros != 0) put(writer, zeros, 5);

    int i = 0;
    while (i < count && (codes[i] & 1) == 0) ++i;
    if (i < count) {
        int current = (codes[i] >> 1) & 1;
        int run = 0;
        put(writer, current, 1);
        for (; i < count; ++i) {
            if ((codes[i] & 1) == 0) continue;
            int sign = (codes[i] >> 1) & 1;
            if (sign != current) {
                put(writer, run, 3);
                current = sign;
                run = 0;
            }
            run = run + 1;
            if (run == 7) {
                put(writer, 7, 3);
                run = 0;
            }
        }
        if (run != 0) put(writer, run, 3);
    }
}

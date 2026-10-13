// Observed receiveControlPacket__FPiib at 0x00132d40, 2596 bytes, a local (file-static) function of
// the net code. Arguments: a packet (bytes), the packet length in bytes and a flag. Names below
// for the partner fields are offsets into one partner record (stride 17632, base `partners`, count
// `nmPartners`); the record P = partners + 17632 * index. Only offsets are claimed.
// Packet header: +4 word `now` (frame number), +8 sender index, +9 a byte (`tag`), +10 count of frames
// carried, +11 signed byte `ahead`; the frame data starts at +12.
//   1. Local index: 0 when netNetworkMode is 0, else the index of the first partner whose byte at
//      +17620 is nonzero (-1 when none or nmPartners is not positive).
//   2. The packet is dropped when P+17620 or P+17621 is nonzero, when P+17622 is nonzero and the flag
//      is 0, or when P+17623 is nonzero. Otherwise P+17612 = 0 and P+17616 = 1. With the flag clear
//      P+17608 = now - tag; with the flag set and local index 0 a byte at the 0x0048681c array
//      slot [tag] is set to 1. P+17604 = now - P+17608. When P+17596 (the last frame held) is not below
//      now the packet only (flag set and local index nonzero) sets P+17624 = P+17596 + 1.
//   3. P+17600 becomes max(P+17600, now + ahead). start = now - count + 1; the packet is dropped
//      when last + 1 < start. skip = last - start + 1 frames are already held, fresh = count - skip.
//   4. A buffer of fresh 12-byte entries is filled. With the flag set it is a memcpy of the
//      packet's entries from skip on. With the flag clear the data is a bit stream (16-bit units read
//      big-endian within a word, MSB first; a reader of data pointer and bit position kept in the
//      frame): a 16-bit value, then runs: 5-bit run length n, the entries covered take the value as
//      the short at +0; when the entries are not all covered, unless n is 31 a 4-bit b XORs the value
//      with 1 << b; then four byte planes (+2, +3, +4, +5) are produced by analDecompress with
//      the states P+16+32, +48, +0 and +16; then the word at +8 per entry is a running short at
//      P+16+66 changed by a velocity at +68 (runs of n entries; after a run, unless 31, a bit
//      selects a 9-bit signed delta (added to the value, also stored as the velocity) or a 16-bit
//      absolute value; that entry takes the value); then bit 0 of that word is set from a run-length
//      bit stream (initial bit, runs of 5-bit lengths, the bit toggling after a run unless it was 31).
//      Finally 16 bits are skipped, the word at P+16+72 grows by fresh and the data pointer advances by
//      (bit position + 7) >> 3 (the memcpy path advances count * 12).
//   5. For each frame f from last + 1 to now: record f & 255 of 24 bytes at P + 172 + 24 * (f & 255)
//      gets f at +0, the 12-byte entry at +4 and zero at +20 (P+192). P+17596 = now; when the flag
//      is set and the local index is nonzero P+17624 = now + 1.
//   6. The data pointer is rounded up to 4 and a word is read; when it is 0xbd40 the next 12 bytes
//      are a state packet whose first byte selects slot P+6316+12*byte; when the local partner's
//      slot has the same first word compareStatePackets(slot, packet, index) runs; the pointer
//      moves 12 further. Then, until the end of the packet: pairs [byte d][byte n][n bytes]: the
//      record (now - d) & 255 gets, when its word at +20 (P+192+24 r) is zero, the write position
//      P+17580 stored at +16 (P+188+24 r) and n at +20; the n bytes are appended to the 8192-byte ring at
//      P+9388 and the position advances (mask 0x1fff). Nonzero records skip their n bytes.
// Inferred: a lock-step input history receive with run-length and prediction coded entries and a
// separate byte stream of per-frame side data. Unknown: names of the entry fields, what the 0x0048681c
// array is, and the meaning of P+17600/17604/17608/17624 beyond the arithmetic above. Nothing here
// ran on the EE.
extern "C" {
extern signed char netNetworkMode;
extern int nmPartners;
extern unsigned char partners[];
extern unsigned char norrath_net_flag_bytes_48681c[];
void* memcpy(void* destination, const void* source, unsigned long count);
void compareStatePackets__FP11statePacketT0i(void* held, void* received, int partner);
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(void* self, void* reader, void* state, unsigned char* out, int count, int skip);
}

namespace {
struct BitBufferReader {
    unsigned char* data;
    int position;
};
inline unsigned window(BitBufferReader* r, int position) {
    unsigned char* a = r->data + (position >> 4) * 2;
    unsigned w = static_cast<unsigned>(*reinterpret_cast<unsigned short*>(a)) << 16 | *reinterpret_cast<unsigned short*>(a + 2);
    return w << (position & 15);
}
inline unsigned read_bits(BitBufferReader* r, int bits) {
    int p = r->position;
    r->position = p + bits;
    return window(r, p) >> (32 - bits);
}
inline int read_signed(BitBufferReader* r, int bits) {
    int p = r->position;
    r->position = p + bits;
    return static_cast<int>(window(r, p)) >> (32 - bits);
}
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
inline void copy12(unsigned char* to, const unsigned char* from) {
    word(to, 0) = *reinterpret_cast<const int*>(from);
    word(to, 4) = *reinterpret_cast<const int*>(from + 4);
    word(to, 8) = *reinterpret_cast<const int*>(from + 8);
}
}  // namespace

extern "C" void receiveControlPacket__FPiib(int* packet_words, int length, bool flag) {
    unsigned char* packet = reinterpret_cast<unsigned char*>(packet_words);
    int local = 0;
    if (netNetworkMode != 0) {
        local = -1;
        for (int i = 0; i < nmPartners; ++i) {
            if (partners[i * 17632 + 17620] != 0) {
                local = i;
                break;
            }
        }
    }
    int index = packet[8];
    unsigned char* end = packet + length;
    int now = word(packet, 4);
    unsigned char* P = partners + index * 17632;
    if (P[17620] != 0) return;
    if (P[17621] != 0) return;
    if (P[17622] != 0 && !flag) return;
    if (P[17623] != 0) return;
    word(P, 17612) = 0;
    word(P, 17616) = 1;
    unsigned char* data = packet + 10;
    if (flag) {
        if (local == 0) norrath_net_flag_bytes_48681c[packet[9]] = 1;
    } else {
        word(P, 17608) = now - packet[9];
    }
    word(P, 17604) = now - word(P, 17608);
    int last = word(P, 17596);
    if (!(last < now)) {
        if (flag && local != 0) word(P, 17624) = word(P, 17596) + 1;
        return;
    }
    int count = packet[10];
    int ahead = static_cast<signed char>(packet[11]);
    data = packet + 12;
    int start = now - count + 1;
    int furthest = word(P, 17600);
    if (!(now + ahead < furthest)) furthest = now + ahead;
    bool gap = last + 1 < start;
    word(P, 17600) = furthest;
    if (gap) return;
    int skip = last - start + 1;
    int fresh = count - skip;
    unsigned char* entries = static_cast<unsigned char*>(__builtin_alloca((fresh * 12 + 15) >> 4 << 4));
    if (flag) {
        memcpy(entries, data + skip * 12, fresh * 12);
        data += count * 12;
    } else {
        BitBufferReader reader;
        unsigned char* state = P + 16;
        unsigned value = *reinterpret_cast<unsigned short*>(data);
        reader.data = data;
        reader.position = 21;
        unsigned run = *reinterpret_cast<unsigned short*>(data + 2) << 16 | *reinterpret_cast<unsigned short*>(data + 4);
        run >>= 27;
        int k = -skip;
        int offset = -skip * 12;
        for (;;) {
            for (int i = static_cast<int>(run); i > 0; --i) {
                if (k >= 0) *reinterpret_cast<unsigned short*>(entries + offset) = static_cast<unsigned short>(value);
                offset += 12;
                ++k;
            }
            if (k == fresh) break;
            if (run != 31) value ^= 1u << read_bits(&reader, 4);
            run = read_bits(&reader, 5);
        }
        unsigned char* plane = static_cast<unsigned char*>(__builtin_alloca((fresh + 15) >> 4 << 4));
        analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(state, &reader, state + 32, plane, fresh, skip);
        for (int i = 0; i < fresh; ++i) entries[i * 12 + 2] = plane[i];
        analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(state, &reader, state + 48, plane, fresh, skip);
        for (int i = 0; i < fresh; ++i) entries[i * 12 + 3] = plane[i];
        analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(state, &reader, state, plane, fresh, skip);
        for (int i = 0; i < fresh; ++i) entries[i * 12 + 4] = plane[i];
        analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(state, &reader, state + 16, plane, fresh, skip);
        for (int i = 0; i < fresh; ++i) entries[i * 12 + 5] = plane[i];
        unsigned short& position = *reinterpret_cast<unsigned short*>(state + 66);
        unsigned short& velocity = *reinterpret_cast<unsigned short*>(state + 68);
        int total = fresh + skip;
        k = 0;
        for (;;) {
            run = read_bits(&reader, 5);
            for (int i = static_cast<int>(run); i > 0; --i) {
                if (!(k < skip)) {
                    position = static_cast<unsigned short>(position + velocity);
                    word(entries, (k - skip) * 12 + 8) = static_cast<short>(position);
                }
                ++k;
            }
            if (k == total) break;
            if (run != 31) {
                bool before = k < skip;
                if (read_bits(&reader, 1)) {
                    if (before) {
                        reader.position += 9;
                    } else {
                        int delta = read_signed(&reader, 9);
                        position = static_cast<unsigned short>(position + delta);
                        velocity = static_cast<unsigned short>(delta);
                    }
                } else {
                    if (before) {
                        reader.position += 16;
                    } else {
                        position = static_cast<unsigned short>(read_signed(&reader, 16));
                    }
                }
                if (!before) word(entries, (k - skip) * 12 + 8) = static_cast<short>(position);
                ++k;
                if (!(k < total)) break;
            }
        }
        unsigned bit = read_bits(&reader, 1);
        k = 0;
        for (;;) {
            run = read_bits(&reader, 5);
            for (int i = static_cast<int>(run); i > 0; --i) {
                if (!(k < skip)) {
                    int& w = word(entries, (k - skip) * 12 + 8);
                    w = (w & ~1) | static_cast<int>(bit);
                }
                ++k;
            }
            if (k == total) break;
            if (run != 31) bit = bit == 0;
        }
        reader.position += 16;
        word(state, 72) += fresh;
        data += (reader.position + 7) >> 3;
    }
    for (int f = last + 1; f <= now; ++f) {
        int slot = f & 0xff;
        unsigned char* record = P + 172 + slot * 24;
        copy12(record + 4, entries + (f - (last + 1)) * 12);
        word(record, 0) = f;
        word(record, 20) = 0;
    }
    word(P, 17596) = now;
    if (flag && local != 0) word(P, 17624) = word(P, 17596) + 1;
    data = reinterpret_cast<unsigned char*>((reinterpret_cast<unsigned long>(data) + 3) & ~3ul) + 4;
    if (*reinterpret_cast<int*>(data - 4) == 0xbd40) {
        unsigned char* partner = partners + local * 17632;
        copy12(P + 6316 + data[0] * 12, data);
        if (word(partner, 6316 + data[0] * 12) == *reinterpret_cast<int*>(data)) {
            compareStatePackets__FP11statePacketT0i(partner + 6316 + data[0] * 12, data, index);
        }
        data += 12;
    }
    if (data == end) return;
    do {
        int r = (now - data[0]) & 0xff;
        int n = data[1];
        unsigned char* bytes = data + 2;
        if (word(P, 192 + r * 24) == 0) {
            int position = word(P, 17580);
            word(P, 188 + r * 24) = position;
            word(P, 192 + r * 24) = n;
            for (int i = 0; i < n; ++i) P[9388 + ((position + i) & 0x1fff)] = bytes[i];
            word(P, 17580) = (position + n) & 0x1fff;
        }
        data = bytes + n;
    } while (data != end);
}

// Observed modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi at 0x00168410, 1204 bytes.
// Arguments: a packet record, a model header, a signed flag byte, a table of 8-byte halo
// definitions and a count. No result. Nothing is done unless bit 0x20 of the model half at +16
// is set. The packet record is four words: [0] the write cursor, [1] the address of the DMA
// tag of the packet being built, [2] the address of the current GIF tag, [3] the address of
// the current VIF DIRECT word.
//
// 1. A state packet is started at the cursor: the cursor moves 8 on, zero bytes pad it to an
//    address that is 12 modulo 16, then VIF word 0x50000000 (its quadword count is patched at
//    the end), a GIF tag 0x1000000000008000 (loop count patched) with register word 14 (A+D),
//    and A+D entries (data, register): (0, 26), (72, 27), (72 if the flag is nonzero else 68,
//    66), (0x71001, 71). The GIF loop count, the VIF count ((end - vif - 4) / 16) and the DMA
//    tag (quadword count, id 1) are filled in for the state packet.
// 2. For each halo definition (bytes 0..3 a colour-like quad, float at +4): a 16-byte-aligned
//    chunk is written: DMA tag (count, id 1) at its start, VIF words 0x13000000, 0x6002c000,
//    a word 1, the float, VIF word 0x6e01c002, the four bytes, VIF word 0x1400002a, then zero
//    padding to a multiple of 16. Then the model's first level table (model + 40 + index *
//    (4 * count + 4), count the signed byte at +18; index is 0 lowered to byte(+38) - 1 when
//    that is smaller) is walked with a mask that has every bit set except bit "count":
//    each maximal run of table addresses with set bits is emitted, at the clear bit, as a REF
//    tag (0x30000000, run start in the high word, (end - start) / 16 in the low) preceded by an
//    empty second word, unless the run is empty.
// 3. A closing state packet restores: a chunk at the cursor with VIF words 0x13000000,
//    0x6001c000, 0x1400002a, padded to 12 modulo 16, then VIF DIRECT, a GIF A+D tag, entries
//    (1, 26), (the 64-bit word at model +24, 66), (0x507f5, 71), padded to a multiple of 16,
//    and the DMA tag written with its count.
// Register numbers are as stored; GS register names are not asserted here. Unknown: what the
// halo fields and the model word at +24 mean.
struct Packet {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* gifTag;
    unsigned char* vif;
};
struct _modelHeader;
struct HaloDef;

namespace {
inline void store64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}
inline void store32(unsigned char* where, unsigned value) {
    *reinterpret_cast<unsigned*>(where) = value;
}
inline unsigned long long load64(const unsigned char* where) {
    return *reinterpret_cast<const unsigned long long*>(where);
}
inline unsigned load32(const unsigned char* where) {
    return *reinterpret_cast<const unsigned*>(where);
}
inline int loop_count(const unsigned char* end, const unsigned char* tag) {
    return ((static_cast<int>(end - tag) >> 3) - 2) >> 1;
}
inline unsigned char* pad_to(unsigned char* at, unsigned residue) {
    while ((reinterpret_cast<__UINTPTR_TYPE__>(at) & 0xfu) != residue) *at++ = 0;
    return at;
}
}  // namespace

extern "C" void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(Packet* packet, _modelHeader* header, char flag,
                                                                   HaloDef* halos, int count) {
    const unsigned char* model = reinterpret_cast<const unsigned char*>(header);
    if ((*reinterpret_cast<const unsigned short*>(model + 16) & 0x20) == 0) return;

    unsigned char* dma = packet->cursor;
    packet->start = dma;
    unsigned char* at = dma + 8;
    packet->cursor = at;
    unsigned char* vif = pad_to(at, 12);
    unsigned char* tag = vif + 4;
    store32(vif, 0x50000000u);
    store64(tag, 0x1000000000008000ull);
    store64(tag + 8, 14ull);
    packet->gifTag = tag;
    store64(vif + 28, 26ull);
    store64(vif + 20, 0ull);
    packet->vif = vif;
    store64(vif + 44, 27ull);
    store64(vif + 36, 72ull);
    packet->cursor = vif + 52;
    if (flag != 0) {
        store64(vif + 60, 66ull);
        store64(vif + 52, 72ull);
    } else {
        store64(vif + 52, 68ull);
        store64(vif + 60, 66ull);
    }
    packet->cursor = vif + 68;
    unsigned char* cursor = packet->cursor;
    unsigned char* end = cursor + 16;
    store64(cursor + 8, 71ull);
    store64(cursor, 0x71001ull);
    unsigned long long gif = load64(packet->gifTag);
    unsigned vifWord = load32(packet->vif);
    store64(packet->gifTag, static_cast<unsigned long long>(static_cast<long long>(loop_count(end, packet->gifTag))) | gif);
    store32(packet->vif, vifWord | static_cast<unsigned>(((end - packet->vif) - 4) >> 4));
    store64(packet->start, static_cast<unsigned long long>(static_cast<long long>(loop_count(end, packet->start))) |
                               0x10000000ull);
    packet->cursor = end;

    unsigned char* tail = end;
    if (count > 0) {
        const unsigned char* halo = reinterpret_cast<const unsigned char*>(halos);
        unsigned char* chunk = end;
        for (int remaining = count; remaining != 0; remaining--, halo += 8) {
            packet->cursor = chunk + 28;
            store32(chunk + 8, 0x13000000u);
            store32(chunk + 12, 0x6002c000u);
            store32(chunk + 16, 1u);
            *reinterpret_cast<float*>(chunk + 20) = *reinterpret_cast<const float*>(halo + 4);
            packet->start = chunk;
            store32(chunk + 24, 0x6e01c002u);
            chunk[28] = halo[0];
            chunk[29] = halo[1];
            chunk[30] = halo[2];
            chunk[31] = halo[3];
            store32(chunk + 32, 0x1400002au);
            packet->cursor = chunk + 36;
            unsigned char* padded = pad_to(chunk + 36, 0);
            unsigned char* begin = packet->start;
            packet->cursor = padded;
            store64(begin, static_cast<unsigned long long>(static_cast<long long>(loop_count(padded, begin))) |
                               0x10000000ull);
            chunk = padded;

            int levels = static_cast<signed char>(model[38]) - 1;
            int index = 0;
            if (levels < index) index = levels;
            int segments = static_cast<signed char>(model[18]);
            const unsigned char* table = model + 40 + index * (segments * 4 + 4);
            unsigned addr = *reinterpret_cast<const unsigned*>(table);
            long long mask = ~(1ll << (segments & 63));
            if (segments >= 0) {
                const unsigned* entry = reinterpret_cast<const unsigned*>(table);
                for (int i = 0; i <= segments; i++, entry++) {
                    bool clear = ((static_cast<int>(mask) ^ 1) & 1) != 0;
                    mask >>= 1;
                    if (clear) {
                        unsigned stop = *entry;
                        if (addr != stop) {
                            unsigned char* next = chunk + 16;
                            packet->cursor = next;
                            store64(chunk + 8, 0ull);
                            packet->start = chunk;
                            long long run = static_cast<int>(stop - addr) >> 4;
                            store64(chunk, (static_cast<unsigned long long>(addr) << 32 | static_cast<unsigned long long>(run)) |
                                               0x30000000ull);
                            chunk = next;
                        }
                        addr = entry[1];
                    }
                }
            }
        }
        tail = chunk;
    }

    unsigned char* cl = tail;
    unsigned char* body = cl + 24;
    store32(cl + 8, 0x13000000u);
    store32(cl + 12, 0x6001c000u);
    store32(cl + 20, 0x1400002au);
    packet->start = cl;
    store32(cl + 16, 0u);
    packet->cursor = body;
    unsigned char* v = pad_to(body, 12);
    unsigned char* gtag = v + 4;
    unsigned char* entry1 = v + 20;
    unsigned char* stop = v + 68;
    store64(gtag, 0x1000000000008000ull);
    store64(gtag + 8, 14ull);
    store64(entry1, 1ull);
    store64(entry1 + 8, 26ull);
    store64(v + 44, 66ull);
    store64(v + 36, load64(model + 24));
    store64(v + 60, 71ull);
    store64(v + 52, 0x507f5ull);
    unsigned long long old = load64(gtag);
    store64(gtag, static_cast<unsigned long long>(static_cast<long long>(loop_count(stop, gtag))) | old);
    store32(v, static_cast<unsigned>(((stop - v) - 4) >> 4) | 0x50000000u);
    packet->vif = v;
    packet->gifTag = gtag;
    packet->cursor = stop;
    unsigned char* done = pad_to(stop, 0);
    unsigned char* begin = packet->start;
    packet->cursor = done;
    store64(begin, static_cast<unsigned long long>(static_cast<long long>(loop_count(done, begin))) | 0x10000000ull);
}

// Observed drawPS__FR6PacketP14ParticleSystem at 0x00185c38 (a local function), 1072 bytes.
// a0 = reference to a packet record of three words, a1 = the particle system, read as
// sixteen 16-byte ranges in four groups of four (group offset 64 * group, as in
// frameNumWrappedPS). Packet words: +0 write cursor, +4 address of the open DMA tag, +8 the
// address of the last GIF tag. All constants below are the observed words.
// For group g = 0, 1, 2:
//  Setup (the cursor is re-read from the packet each time):
//   g == 0 or 1: the word at cursor+8 = 0x11000000 and the open tag becomes the cursor; the
//     cursor moves 12 bytes forward and zero bytes are written until (cursor & 15) == 12. The
//     GIF tag dword at (cursor+4) is 0x1000000000008000 with count (a3-a1)/8-2 >> 1 or-ed in
//     (here 1), followed by the dword 14. Two address-and-data dwords at cursor+20 are
//     (68, 66) for g == 0 and (72, 66) for g == 1. The packet word +8 takes the GIF tag
//     address, the word 0x50000002 is stored at cursor (before the GIF tag), the cursor ends
//     at cursor+36, and the open DMA tag dword = (((end - tag)/8 - 2) >> 1) | 0x10000000.
//   g == 2: the open tag becomes the cursor, the dword there is 0x10000000, the cursor moves
//     16 bytes, and the words at +8 and +12 are 0x14000022 and 0x11000000.
//  Ranges (four per group, 16 bytes each: first node, last node, first index, last index;
//  a null first node skips the range). Nodes are 4-byte links in blockList (0x00780080); a
//  node's block is block (0x00700080) + (node - blockList) * 1024 bytes, records are
//  16 bytes. For each node on the chain from the first to the last (the link is the node's
//  first word): start = (node == first ? first_index : 0) & -32; end = node != last ? 256 :
//  ((last_index + 30) & -32) - 1; a 16-byte entry is written at the cursor with the dword at
//  +8 zero and the dword at +0 = (block address of record `start` << 32) | (end - start) |
//  0x30000000; the open tag is that entry and the cursor advances 16.
//  After the chain: k = 31 - (last_index & 31). The open tag is the entry's next 16 bytes
//  and the cursor is entry + 24. If k < 30, the word at entry+24 is
//  0x60000000 | (k << 17) | 0x02000000 | 0x0c000000 | m where m = (30 - k) * 2 + 576 when bit
//  0 of (last_index - 2) >> 5 is zero, else (30 - k) * 2 + 128, and k * 2 zero words follow.
//  Then the words 0x14000017 and 0x11000000 are written at the cursor, the cursor moves 8,
//  zero bytes pad it to a multiple of 16, and the open tag's dword becomes
//  (((cursor - tag)/8 - 2) >> 1) | 0x10000000.
// Inferred only: these are DMA ref/cnt tags and VIF/GIF words for a particle batch. The
// meaning of the unpack word is not established. Fresh compilation is not an original-byte
// match.
struct Packet {
    unsigned char* cursor;
    unsigned char* tag;
    unsigned char* gif;
};
struct Range {
    unsigned* first_node;
    unsigned* last_node;
    int first_index;
    int last_index;
};

extern "C" {
extern unsigned char block[];
extern unsigned blockList[];
}

namespace {
inline void store32(unsigned char* address, unsigned value) { *reinterpret_cast<unsigned*>(address) = value; }
inline void store64(unsigned char* address, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(address) = value;
}
}

extern "C" void drawPS__FR6PacketP14ParticleSystem(Packet* packet, Range* system) {
    for (int group = 0; group < 3; ++group) {
        if (group == 1 || group == 0) {
            unsigned char* start = packet->cursor;
            unsigned char* t = start + 12;
            packet->tag = start;
            store32(start + 8, 0x11000000u);
            packet->cursor = t;
            while ((reinterpret_cast<__UINTPTR_TYPE__>(t) & 15u) != 12u) *t++ = 0;
            unsigned char* gif = t + 4;
            unsigned char* open_tag = packet->tag;
            unsigned char* data = t + 20;
            store64(gif, 0x1000000000008000ull);
            store64(gif + 8, 14ull);
            store64(data, group == 1 ? 72ull : 68ull);
            store64(data + 8, 66ull);
            unsigned char* end = t + 36;
            const int gif_count = (static_cast<int>((end - gif) >> 3) - 2) >> 1;
            const int dma_count = (static_cast<int>((end - open_tag) >> 3) - 2) >> 1;
            const unsigned long long gif_word = *reinterpret_cast<unsigned long long*>(gif);
            store64(gif, static_cast<unsigned long long>(static_cast<long long>(gif_count)) | gif_word);
            store64(open_tag, static_cast<unsigned long long>(static_cast<long long>(dma_count)) | 0x10000000ull);
            packet->gif = gif;
            store32(t, 0x50000002u);
            packet->cursor = end;
        } else {
            unsigned char* start = packet->cursor;
            unsigned char* next = start + 16;
            packet->tag = start;
            store64(start, ((((next - start) >> 3) - 2) >> 1) | 0x10000000ull);
            packet->cursor = next;
            store32(start + 8, 0x14000022u);
            store32(start + 12, 0x11000000u);
        }
        Range* range = reinterpret_cast<Range*>(reinterpret_cast<unsigned char*>(system) + group * 64);
        for (int slot = 0; slot < 4; ++slot, ++range) {
            unsigned* const first = range->first_node;
            if (first == 0) continue;
            unsigned* const last = range->last_node;
            const int first_index = range->first_index;
            const int last_index = range->last_index;
            unsigned* node = first;
            unsigned char* entry;
            for (;;) {
                int begin = (node == first) ? first_index : 0;
                begin &= -32;
                int stop = (node != last) ? 256 : ((last_index + 30) & -32) - 1;
                entry = packet->cursor;
                const int count = stop - begin;
                const unsigned offset = ((static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(node) -
                                                                reinterpret_cast<__UINTPTR_TYPE__>(blockList))
                                          << 6) + static_cast<unsigned>(begin))
                                        << 4;
                store64(entry + 8, 0ull);
                packet->tag = entry;
                const unsigned long long address =
                    static_cast<unsigned long long>(offset + static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block)))
                    << 32;
                packet->cursor = entry + 16;
                store64(entry, address | static_cast<unsigned long long>(static_cast<long long>(count)) | 0x30000000ull);
                if (node == last) break;
                node = reinterpret_cast<unsigned*>(*node);
            }
            const int k = 31 - (last_index & 31);
            packet->tag = entry + 16;
            packet->cursor = entry + 24;
            if (k < 30) {
                int shifted = (last_index - 2) >> 5;
                int words = (30 - k) << 1;
                int selected = (shifted & 1) == 0 ? words + 576 : words + 128;
                unsigned command = static_cast<unsigned>(selected) | 0x60000000u |
                                   ((static_cast<unsigned>(k) << 17) | 0x02000000u) | 0x0c000000u;
                store32(entry + 24, command);
                unsigned char* fill = entry + 28;
                packet->cursor = fill;
                int zero_words = k << 1;
                if (zero_words > 0) {
                    for (int n = zero_words; n != 0; --n) {
                        store32(fill, 0u);
                        fill += 4;
                    }
                    packet->cursor = fill;
                }
            }
            unsigned char* cur = packet->cursor;
            store32(cur, 0x14000017u);
            store32(cur + 4, 0x11000000u);
            unsigned char* pad = cur + 8;
            packet->cursor = pad;
            while ((reinterpret_cast<__UINTPTR_TYPE__>(pad) & 15u) != 0u) *pad++ = 0;
            packet->cursor = pad;
            unsigned char* tag = packet->tag;
            store64(tag, ((((pad - tag) >> 3) - 2) >> 1) | 0x10000000ull);
        }
    }
}

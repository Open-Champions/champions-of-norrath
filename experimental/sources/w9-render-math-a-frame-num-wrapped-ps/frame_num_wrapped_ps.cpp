// Observed frameNumWrappedPS__FP14ParticleSystem at 0x00181438, 276 bytes (a local
// function). The argument points at sixteen 16-byte ranges laid out as four groups of four
// (the offset of the group is 64 bytes times the group index, the range stride is 16). Each
// range holds four words: first node, last node, first index, last index.
//  1. A range with a null first node is skipped.
//  2. Nodes are 4-byte links in blockList (0x00780080, 128 nodes); the node's first word is
//     the next node. A node maps to a 4096-byte block of block (0x00700080) at
//     (node - blockList) * 4096 bytes (node stride 4). Records in a block are 16 bytes.
//  3. Walk: stop when node == last node and index == last index. Otherwise flip bit 15 of the
//     halfword at record offset 6 (add -32768, 16-bit store), increment the index, and if the
//     new index modulo 32 is 31: stop skipping when the new index is 255, else add two. At
//     255 the walk ends if the node is the last node; otherwise it follows the link and
//     restarts at index 1 of the next block.
// Indices 31 and 32 (and 63 and 64, ...) are therefore never visited. Unknown: why. Whether
// the halfword is a frame counter is inferred from the name only. Fresh compilation is not an
// original-byte match.
struct ParticleRange {
    unsigned* first_node;
    unsigned* last_node;
    int first_index;
    int last_index;
};

extern "C" {
extern unsigned char block[];
extern unsigned blockList[];

void frameNumWrappedPS__FP14ParticleSystem(ParticleRange* system) {
    for (int group = 0; group < 4; ++group) {
        ParticleRange* range = reinterpret_cast<ParticleRange*>(reinterpret_cast<unsigned char*>(system) + group * 64);
        for (int slot = 0; slot < 4; ++slot, ++range) {
            unsigned* node = range->first_node;
            if (node == 0) continue;
            unsigned* const last = range->last_node;
            int index = range->first_index;
            const int last_index = range->last_index;
            unsigned char* base = block + static_cast<unsigned>(node - blockList) * 4096u;
            for (;;) {
                if (node == last && index == last_index) break;
                unsigned short* half = reinterpret_cast<unsigned short*>(base + index * 16 + 6);
                ++index;
                *half = static_cast<unsigned short>(*half - 32768);
                if ((index & 31) != 31) continue;
                if (index != 255) {
                    index += 2;
                    continue;
                }
                if (node == last) break;
                node = reinterpret_cast<unsigned*>(*node);
                index = 1;
                base = block + static_cast<unsigned>(node - blockList) * 4096u;
            }
        }
    }
}
}

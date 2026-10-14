// Observed: vqDecompress__FP8_texturei at 0x001712f8, 4676 bytes (a local symbol in the image).
// Arguments: texture, extra (int). It returns 0 when 64 free cache-map entries cannot be claimed
// and 1 when the whole stream has been decoded. The texture's half at +0 is the width, the half at
// +2 the height, the word at +16 points at the stream, and the word at +20 receives the first block
// address on success. The routine is time-sliced in the same way as the table decoder in
// texDecompressThread__FPv (another unit): scratchpad cursor 0x70000000, GS upload packets built in
// the scratchpad, flushed by putBlock__FP8_textureP6PacketPPP9uncompMapPPii, waits on the timer word
// at 0x10000000 using SleepThread.
// Steps, in order:
//   1. If the word at +16 is nonzero the routine reads the word at +16 of the 64-byte slots below
//      the texture until it finds a zero (the result is not used; kept as a volatile read loop).
//      The cursor becomes 0x70000000 and texDecodeStarted becomes the texture.
//   2. Claim: starting at uncompSpin, the 8-byte uncompMap entries {owner, next} are walked, as in
//      the table decoder, until 64 entries are taken. An entry is taken when it has no owner or the
//      owner's word at +28 is not greater than drawFrameNm - 3; a taken entry with an owner has the
//      owner's +20 word cleared and its linked entries cleared. Stepping wraps at 1280. After 1279
//      failed steps texDecodeStarted is cleared and 0 is returned.
//   3. texturesAreDecompressing = 1. The stream word at +0 points at a header. A GS upload packet
//      goes to the cursor: GIF tag 0x1000000000008000 (loop count 4 added), 14, then data/register
//      pairs (1<<48, 80), (0, 81), (0x1000000010, 82), (0, 83) and the closing pair
//      (0x0800000000008040, 0). Register numbers 80 to 83 are 0x50 to 0x53, which look like the GS
//      BITBLTBUF, TRXPOS, TRXREG and TRXDIR registers; that is inferred from numbers only. 1024 bytes
//      follow, copied word by word from the header's first 256 words (after its first two halves)
//      with the index mapping dst = (j & 7) + ((i >> 1) << 5) + ((j >> 3) << 4) + ((i & 1) << 3) for
//      source word 16*i + j. curVqPalette = the header pointer. The header begins with two halves,
//      P (words in the palette block) and N (codebook entry count).
//   4. Two code lists follow the palette block. Each is a count word then pairs of words, ending at a
//      pair whose second word is -1; list 1 is processed by processHuffman__FPUi, then N entries of
//      four codes each are decoded through the 32-bit window and the gbits / gsubtracts tables into
//      huffmanTemp (N * 4 bytes), or huffmanTemp is zeroed when list 1's first word is 0. Bits are
//      taken from a reader {64-bit buffer, bit count, byte pointer} filled one byte at a time. List 2
//      is processed by processHuffman afterwards.
//   5. A second GS packet header with width (w + 127) >> 7 in bits 48 and up is placed after the
//      palette block, 32 bytes long. The block-column and block-row counts are (w + 15) / 16 and
//      (h + 15) / 16; the field widths are the smallest b >= 1 with count < 1 << b.
//   6. A reader starting at stream + 4 yields rectangles (A, B, C, D) of those widths. C < A ends the
//      stream: putBlock is called with the last flag set until the cursor is back at 0x70000000, the
//      texture's +20 word becomes the output block address, the chain end is cleared,
//      texDecodeStarted = 0, and 1 is returned. Otherwise a packet with TRXREG, TRXPOS, TRXDIR and an
//      IMAGE tag covering (C - A) * (D - B) 16x16 byte blocks follows, then for each row B..D-1 and
//      each block A..C-1: one bit selects inline data or a 24-bit reference (3-bit bit offset,
//      21 bits of address); a reference is resolved through texturePageTable[extra] (pages below 9
//      through texturePageSegLoc and the word at +124 of world, others through texturePageLoc), read
//      from a second reader which first skips the bit offset and one more bit. Each 16x16 block is
//      built from 2x2 groups of four huffmanTemp bytes (index decoded from list 2, or huffmanTemp
//      bytes 0 to 3 when list 2's first word is 0), clipped to the texture width and height, then
//      the 256 bytes are permuted through unscrambleTable into eight 32-byte lines of the row.
//      Whenever the cursor is more than 4096 bytes past 0x70000000, putBlock is called with a zero
//      flag. Before every row and before several loads the timer word is checked against 9390 (18780
//      when engineFullFrameMode is nonzero) and the thread sleeps while it is above that limit, unless
//      textureUncompBufferLocked or clearing is set.
// Inferred: this is the vector-quantized texture decoder (codebook of 2x2 pixel groups, indexed 8-bit
// blocks, 256-entry palette); the names are from the symbol table, the roles of the fields are not
// proven. The count loop over gbits replaces the original multimedia compare, pack and leading-count
// sequence; it is written out for one group of four lanes at a time and returns the same count.
// Unknown: the stream producer, the texture structure beyond the fields named above, the meaning of
// the GS register values, and what the first dead read loop is for.
#define VQ_SCRATCH (reinterpret_cast<unsigned char*>(0x70000000u))
#define VQ_TIMER (*reinterpret_cast<volatile unsigned*>(0x10000000u))

struct TexDecodePacket {
    unsigned char* cursor;
    unsigned char* unknown;
    unsigned char* last;
};

struct BitReader {
    unsigned long long bits;
    int count;
    const unsigned char* pointer;
};

extern "C" {
extern int uncompSpin;
extern int uncompCache;
extern int drawFrameNm;
extern unsigned char* texDecodeStarted;
extern unsigned char* curVqPalette;
extern unsigned char textureUncompBufferLocked;
extern unsigned char clearing;
extern unsigned char texturesAreDecompressing;
extern int engineFullFrameMode;
extern unsigned char uncompMap[];
extern unsigned char unscrambleTable[];
extern unsigned char huffmanTemp[];
extern int gsubtracts[];
extern int gbits[];
extern signed char texturePageTable[];
extern unsigned char texturePageSegLoc[];
extern unsigned texturePageLoc[];
extern unsigned char* world;

int SleepThread();
void* memset(void* destination, int value, unsigned long count);
void processHuffman__FPUi(const unsigned char* list);
void putBlock__FP8_textureP6PacketPPP9uncompMapPPii(void* texture, TexDecodePacket* packet, void*** list, int** slot, int last);
int vqDecompress__FP8_texturei(void* texture, int extra);
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_dword(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}
inline unsigned long long load_dword(const unsigned char* where) {
    return *reinterpret_cast<const unsigned long long*>(where);
}
inline unsigned char* address(unsigned bits) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
}

// Adds whole bytes below the buffered bits until at least `need` bits are present.
inline void fill(BitReader* reader, int need) {
    while (reader->count < need) {
        unsigned long long byte = *reader->pointer++;
        reader->bits |= byte << (56 - reader->count);
        reader->count += 8;
    }
}
inline int take(BitReader* reader, int amount) {
    int value = static_cast<int>(reader->bits >> (64 - amount));
    reader->bits <<= amount;
    reader->count -= amount;
    return value;
}
// Sleeps while the timer is above the limit, unless the uncompressed buffer is locked or clearing.
inline void wait_for_timer() {
    while (textureUncompBufferLocked == 0 && clearing == 0 &&
           (engineFullFrameMode == 0 ? 9390u : 18780u) < VQ_TIMER) {
        SleepThread();
    }
}
// One table-driven decode step: peeks 32 bits, picks the length from the gbits thresholds, puts the
// low `length` bits of the window back and returns the index.
inline int huffman_index(BitReader* reader) {
    fill(reader, 32);
    const unsigned long long bits = reader->bits;
    const int count = reader->count;
    const int window = static_cast<int>(bits >> 32);
    reader->count = count - 32;
    const int probe = static_cast<int>(static_cast<unsigned>(window) >> 2);
    const int* group = gbits;
    int steps = 0;
    for (;;) {
        int lanes;
        if (group[3] > probe) lanes = 0;
        else if (group[2] > probe) lanes = 1;
        else if (group[1] > probe) lanes = 2;
        else if (group[0] > probe) lanes = 3;
        else lanes = 4;
        steps += lanes;
        group += 4;
        if (lanes != 4) break;
    }
    const int length = gsubtracts[0] - steps;
    reader->bits = bits << 32;
    const unsigned mask = ~(0xffffffffu << length);
    const int index = static_cast<int>(static_cast<unsigned>(window) >> length) - gsubtracts[length];
    if (length != 0) {
        const unsigned long long low = static_cast<unsigned>(window) & mask;
        reader->bits = (reader->bits >> length) | (low << (64 - length));
        reader->count = (count - 32) + length;
    }
    return index;
}
}  // namespace

extern "C" int vqDecompress__FP8_texturei(void* texture_pointer, int extra) {
    unsigned char* texture = static_cast<unsigned char*>(texture_pointer);
    TexDecodePacket packet;
    BitReader codebook_reader;
    BitReader block_reader;
    BitReader reference_reader;
    int output;
    int* slot0;
    void** slot1;
    unsigned char buffer[256];

    if (word(texture, 16) != 0) {
        const unsigned char* probe = texture - 64;
        for (;;) {
            unsigned value = *reinterpret_cast<const volatile unsigned*>(probe + 16);
            probe -= 64;
            if (value == 0) break;
        }
    }
    packet.cursor = VQ_SCRATCH;
    texDecodeStarted = texture;

    {
        int index = uncompSpin;
        int steps = 0;
        int needed = 64;
        const int threshold = drawFrameNm - 3;
        do {
            unsigned char* entry = uncompMap + index * 8;
            bool take_it = word(entry, 0) == 0u || !(threshold < static_cast<int>(word(address(word(entry, 0)), 28)));
            while (!take_it) {
                const int before = steps;
                ++index;
                if (index >= 1280) index = 0;
                ++steps;
                if (!(before < 1279)) {
                    texDecodeStarted = nullptr;
                    return 0;
                }
                entry = uncompMap + index * 8;
                take_it = word(entry, 0) == 0u || !(threshold < static_cast<int>(word(address(word(entry, 0)), 28)));
            }
            if (word(entry, 0) != 0u) {
                unsigned char* owner = address(word(entry, 0));
                unsigned char* slot = uncompMap + ((static_cast<int>(word(owner, 20)) - uncompCache) >> 12) * 8;
                store_word(owner, 20, 0u);
                store_word(slot, 0, 0u);
                for (unsigned next = word(slot, 4); next != 0u; next = word(slot, 4)) {
                    slot = address(next);
                    store_word(slot, 0, 0u);
                }
            }
            {
                const int before = steps;
                ++index;
                if (index >= 1280) index = 0;
                ++steps;
                if (!(before < 1279)) {
                    texDecodeStarted = nullptr;
                    return 0;
                }
            }
            --needed;
        } while (needed > 0);
    }

    texturesAreDecompressing = 1;
    const unsigned char* stream = address(word(texture, 16));
    slot0 = &output;
    {
        unsigned char* base = packet.cursor;
        store_dword(base, 0x1000000000008000ull);
        store_dword(base + 8, 14ull);
        store_dword(base + 16, 1ull << 48);
        store_dword(base + 24, 80ull);
        store_dword(base + 32, 0ull);
        store_dword(base + 40, 81ull);
        store_dword(base + 48, 0x1000000010ull);
        store_dword(base + 56, 82ull);
        store_dword(base + 64, 0ull);
        store_dword(base + 72, 83ull);
        store_dword(base + 80, 0x0800000000008040ull);
        store_dword(base + 88, 0ull);
        packet.last = base;
        packet.cursor = base + 96;
        slot1 = nullptr;
        store_dword(base, load_dword(base) | 4ull);
    }
    const unsigned char* header = address(word(stream, 0));
    curVqPalette = const_cast<unsigned char*>(header);
    const unsigned palette_words = *reinterpret_cast<const unsigned short*>(header);
    const unsigned entry_count = *reinterpret_cast<const unsigned short*>(header + 2);
    const unsigned char* data = header + 4;
    {
        int word_index = 0;
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                const int target = (j & 7) + ((i >> 1) << 5) + ((j >> 3) << 4) + ((i & 1) << 3);
                store_word(packet.cursor, static_cast<unsigned>(target) * 4, word(data, static_cast<unsigned>(word_index) * 4));
                ++word_index;
            }
        }
    }
    data += palette_words * 4;
    packet.cursor += 1024;
    wait_for_timer();

    const unsigned char* list1 = data;
    const unsigned char* walk = list1 + 4;
    while (static_cast<int>(word(walk, 4)) != -1) walk += 8;
    walk += 8;
    const unsigned char* list2 = walk;
    walk = list2 + 4;
    while (static_cast<int>(word(walk, 4)) != -1) walk += 8;
    processHuffman__FPUi(list1);

    codebook_reader.bits = 0;
    codebook_reader.count = 0;
    codebook_reader.pointer = walk + 8;
    if (word(list1, 0) == 0) {
        memset(huffmanTemp, 0, entry_count * 4);
    } else {
        for (int entry = 0; entry < static_cast<int>(entry_count); ++entry) {
            wait_for_timer();
            for (int part = 0; part < 4; ++part) {
                huffmanTemp[entry * 4 + part] = static_cast<unsigned char>(huffman_index(&codebook_reader));
            }
        }
    }
    processHuffman__FPUi(list2);

    {
        unsigned char* second = packet.cursor;
        const int width = *reinterpret_cast<const short*>(texture);
        store_dword(second, 0x1000000000008000ull);
        store_dword(second + 8, 14ull);
        store_dword(second + 16, static_cast<unsigned long long>(static_cast<long long>((width + 127) >> 7)) << 48);
        store_dword(second + 24, 80ull);
        packet.last = second;
        packet.cursor = second + 32;
        store_dword(second, load_dword(second) | 1ull);
    }
    wait_for_timer();

    int column_bits = 1;
    int row_bits = 1;
    {
        const int width = *reinterpret_cast<const short*>(texture);
        const int height = *reinterpret_cast<const short*>(texture + 2);
        const int columns = (width + 15) / 16;
        const int rows_total = (height + 15) / 16;
        if (columns >= 2) {
            do {
                ++column_bits;
            } while (!(columns < (1 << column_bits)));
        }
        if (rows_total >= 2) {
            do {
                ++row_bits;
            } while (!(rows_total < (1 << row_bits)));
        }
    }
    block_reader.bits = 0;
    block_reader.count = 0;
    block_reader.pointer = address(word(texture, 16)) + 4;

    for (;;) {
        int left = 0, top = 0, right = 0, bottom = 0;
        if (column_bits != 0) {
            fill(&block_reader, column_bits);
            left = take(&block_reader, column_bits);
        }
        if (row_bits != 0) {
            fill(&block_reader, row_bits);
            top = take(&block_reader, row_bits);
        }
        if (column_bits != 0) {
            fill(&block_reader, column_bits);
            right = take(&block_reader, column_bits);
        }
        if (row_bits != 0) {
            fill(&block_reader, row_bits);
            bottom = take(&block_reader, row_bits);
        }
        if (right < left) {
            do {
                putBlock__FP8_textureP6PacketPPP9uncompMapPPii(texture, &packet, &slot1, &slot0, 1);
            } while (static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(packet.cursor) - 0x70000000u) > 0);
            store_word(texture, 20, static_cast<unsigned>(output));
            *slot1 = nullptr;
            texDecodeStarted = nullptr;
            return 1;
        }
        const int columns_here = right - left;
        const int rows_here = bottom - top;
        {
            unsigned char* base = packet.cursor;
            store_dword(base, 0x1000000000008000ull);
            store_dword(base + 8, 14ull);
            store_dword(base + 16, static_cast<unsigned long long>((static_cast<long long>(rows_here * 8) << 32) |
                                                                  static_cast<long long>(columns_here * 8)));
            store_dword(base + 24, 82ull);
            store_dword(base + 32, static_cast<unsigned long long>((static_cast<long long>(left * 8) << 32) |
                                                                  (static_cast<long long>(top * 8) << 48)));
            store_dword(base + 40, 81ull);
            store_dword(base + 48, 0ull);
            store_dword(base + 56, 83ull);
            packet.cursor = base + 80;
            const int quadwords = (columns_here * rows_here) << 4;
            store_dword(base + 64, static_cast<unsigned long long>(static_cast<long long>(quadwords)) | 0x8000ull |
                                       0x0800000000000000ull);
            store_dword(base + 72, 0ull);
            packet.last = base;
            store_dword(base, load_dword(base) | 3ull);
        }
        const int row_stride = columns_here * 8;
        for (int row = top; row < bottom; ++row) {
            wait_for_timer();
            unsigned char* row_base = packet.cursor;
            packet.cursor = row_base + columns_here * 256;
            int column = 0;
            for (int block = left; block < right; ++block, ++column) {
                const int width = *reinterpret_cast<const short*>(texture);
                const int height = *reinterpret_cast<const short*>(texture + 2);
                const int first_row = row << 4;
                const int first_column = block << 4;
                const int row_limit = (first_row + 16 < height) ? first_row + 16 : height;
                const int column_limit = (first_column + 16 < width) ? first_column + 16 : width;
                BitReader* active = &block_reader;
                fill(&block_reader, 1);
                if (take(&block_reader, 1) != 0) {
                    fill(&block_reader, 24);
                    const int reference = take(&block_reader, 24);
                    const int bit_offset = reference & 7;
                    const int byte_offset = reference >> 3;
                    const int page = texturePageTable[extra];
                    unsigned source;
                    if (page < 9) {
                        source = word(world, 124) +
                                 (static_cast<unsigned>(texturePageSegLoc[(page << 7) + (byte_offset >> 17)]) << 17) +
                                 (static_cast<unsigned>(byte_offset) & 0x1ffffu);
                    } else {
                        source = texturePageLoc[page] + static_cast<unsigned>(byte_offset);
                    }
                    reference_reader.bits = 0;
                    reference_reader.count = 0;
                    reference_reader.pointer = address(source);
                    if (bit_offset != 0) {
                        fill(&reference_reader, bit_offset);
                        reference_reader.bits <<= bit_offset;
                        reference_reader.count -= bit_offset;
                    }
                    fill(&reference_reader, 1);
                    reference_reader.bits <<= 1;
                    reference_reader.count -= 1;
                    active = &reference_reader;
                }
                const bool coded = word(list2, 0) != 0;
                int pair_row = 0;
                for (int y = first_row; y < row_limit; y += 2, ++pair_row) {
                    int upper = pair_row << 5;
                    int lower = upper + 16;
                    for (int x = first_column; x < column_limit; x += 2, upper += 2, lower += 2) {
                        const unsigned char* group = huffmanTemp;
                        if (coded) group = huffmanTemp + huffman_index(active) * 4;
                        buffer[upper] = group[0];
                        buffer[upper + 1] = group[1];
                        buffer[lower] = group[2];
                        buffer[lower + 1] = group[3];
                    }
                }
                {
                    int source_index = 0;
                    int line_offset = 0;
                    for (int line = 0; line < 8; ++line, line_offset += row_stride) {
                        unsigned char* out = row_base + (column * 8 + line_offset) * 4;
                        for (int pair = 0; pair < 8; ++pair, out += 4) {
                            out[0] = buffer[unscrambleTable[source_index]];
                            out[1] = buffer[unscrambleTable[source_index + 1]];
                            out[2] = buffer[unscrambleTable[source_index + 2]];
                            out[3] = buffer[unscrambleTable[source_index + 3]];
                            source_index += 4;
                        }
                    }
                }
            }
            if (static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(packet.cursor) - 0x70000000u) >= 4097) {
                do {
                    putBlock__FP8_textureP6PacketPPP9uncompMapPPii(texture, &packet, &slot1, &slot0, 0);
                } while (static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(packet.cursor) - 0x70000000u) >= 4097);
            }
        }
    }
}

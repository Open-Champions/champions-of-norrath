// Observed extract_cie_info at 0x002fca70, 268 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. Arguments: a frame-description entry and a result record of five words. In order:
//   * result[0] (the augmentation string) = the entry's address + 13 - the word at entry + 4; the string sits 9 bytes
//     into the common-information record that this entry's back-offset names;
//   * the string must be "" or "eh" or begin with 'z'; anything else returns 0;
//   * the cursor is the byte after the string's terminator. For "eh", result[1] is the four bytes at the cursor
//     (read unaligned) and the cursor moves on by four; otherwise result[1] = 0;
//   * result[2] = unsigned LEB128 at the cursor (decode_uleb128, 0x002fbd18), result[3] = signed LEB128 (decode_sleb128,
//     0x002fbd60);
//   * result[4] = the next byte, which advances the cursor by one;
//   * if the string begins with 'z', a further unsigned LEB128 gives a length by which the cursor then skips.
//     The cursor is returned.
// The two string literals compared are the empty string and "eh" (rodata at 0x0047df20 and 0x0047df28).
// Inferred (from the unwinder's use): result[2] code alignment, result[3] data alignment, result[4] return-address
// column. Names are mine. Unknown: nothing was executed on the EE.
extern "C" {
struct w15_cie_info {
    const char* augmentation;
    unsigned eh_pointer;
    unsigned code_align;
    int data_align;
    unsigned return_column;
};

int strcmp(const char* a, const char* b);
unsigned strlen(const char* s);
const unsigned char* decode_uleb128(const unsigned char* cursor, unsigned* value);
const unsigned char* decode_sleb128(const unsigned char* cursor, int* value);

struct w15_unaligned_word {
    unsigned value;
} __attribute__((packed));

const unsigned char* extract_cie_info(const unsigned char* fde, w15_cie_info* info) {
    info->augmentation = reinterpret_cast<const char*>(fde - *reinterpret_cast<const int*>(fde + 4) + 13);
    const char* aug = info->augmentation;
    if (strcmp(aug, "") != 0 && strcmp(aug, "eh") != 0 && *aug != 'z') return nullptr;

    const unsigned char* cursor = reinterpret_cast<const unsigned char*>(aug) + strlen(aug) + 1;
    if (strcmp(info->augmentation, "eh") == 0) {
        info->eh_pointer = reinterpret_cast<const w15_unaligned_word*>(cursor)->value;
        cursor += 4;
    } else {
        info->eh_pointer = 0;
    }
    cursor = decode_uleb128(cursor, &info->code_align);
    cursor = decode_sleb128(cursor, &info->data_align);
    info->return_column = *cursor++;
    if (*info->augmentation == 'z') {
        unsigned skip;
        cursor = decode_uleb128(cursor, &skip);
        cursor += skip;
    }
    return cursor;
}
}

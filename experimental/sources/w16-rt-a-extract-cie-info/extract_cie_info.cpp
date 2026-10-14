// Observed extract_cie_info at 0x002fca70, 268 bytes, a local routine of the exception runtime. Arguments: a
// frame-description entry and a five-word result record. Steps, in order:
//   * word 0 of the record (a text pointer) = entry address + 13 - the word at entry + 4. The entry's second word is
//     a back offset to the common-information record, whose text begins 9 bytes in.
//   * the text must be empty, "eh", or start with 'z'; otherwise the result is null (the record's word 0 is already
//     stored and nothing else is).
//   * the cursor is the byte after the text's terminator. For "eh" word 1 is the four bytes at the cursor, read
//     unaligned, and the cursor moves by four; otherwise word 1 is 0.
//   * word 2 = unsigned LEB128 at the cursor (decode_uleb128, 0x002fbd18); word 3 = signed LEB128 (decode_sleb128,
//     0x002fbd60); word 4 = the next byte (zero-extended).
//   * for text starting with 'z' one more unsigned LEB128 gives a count that the cursor then skips.
//   The final cursor is returned. The two literals compared against are "" and "eh" (rodata at 0x0047df20/28).
// Inferred from how the unwinder uses the words: code alignment, data alignment, return-address column. Names are
// mine; the image names none of this.
extern "C" {
struct CieInfo {
    const char* text;
    unsigned eh_pointer;
    unsigned code_align;
    int data_align;
    unsigned return_column;
};

int strcmp(const char* left, const char* right);
unsigned strlen(const char* text);
const unsigned char* decode_uleb128(const unsigned char* cursor, unsigned* value);
const unsigned char* decode_sleb128(const unsigned char* cursor, int* value);

struct UnalignedWord {
    unsigned value;
} __attribute__((packed));

const unsigned char* extract_cie_info(const unsigned char* entry, CieInfo* info) {
    const int back = *reinterpret_cast<const int*>(entry + 4);
    info->text = reinterpret_cast<const char*>(entry - back + 13);
    if (strcmp(info->text, "") != 0 && strcmp(info->text, "eh") != 0 && *info->text != 'z') return nullptr;

    const unsigned char* cursor = reinterpret_cast<const unsigned char*>(info->text) + strlen(info->text) + 1;
    if (strcmp(info->text, "eh") == 0) {
        info->eh_pointer = reinterpret_cast<const UnalignedWord*>(cursor)->value;
        cursor += 4;
    } else {
        info->eh_pointer = 0;
    }
    cursor = decode_uleb128(cursor, &info->code_align);
    cursor = decode_sleb128(cursor, &info->data_align);
    info->return_column = *cursor++;
    if (*info->text == 'z') {
        unsigned skip;
        cursor = decode_uleb128(cursor, &skip);
        cursor += skip;
    }
    return cursor;
}
}

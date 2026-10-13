// Observed __frame_state_for at 0x002fd0f0, 440 bytes, in the exception-support part of the runtime library
// (the unwind-table reader that __throw uses). Arguments: a code address and a caller-provided state record
// (1144 bytes, the record __throw keeps on its stack). Returns that record, or null.
//  1. f = find_fde(address). Null: return null.
//  2. insn = extract_cie_info(f, &info). Null: return null. info is a 32-byte record on the stack; the code
//     reads its first word (a string whose first character is compared with 'z', so an augmentation string),
//     its word at offset 4 and its halfword at offset 16.
//  3. A local 1152-byte work record is zeroed. Its word at offset 4 gets info's word at offset 4 and its
//     halfword at offset 1018 gets info's halfword at offset 16.
//  4. The common-information entry that the descriptor points to (the descriptor's word at offset 4 is the
//     distance back to it) is walked from insn to the end of that entry (its length word plus 4):
//     insn = execute_cfa_insn(insn, work, &info, null) while insn is below that end.
//  5. insn then restarts at the descriptor plus 16. When the augmentation string starts with 'z',
//     decode_uleb128(insn, &length) skips the length it reads: insn = returned pointer + length.
//  6. With end = descriptor + its length word + 4 and pc = the descriptor's word at offset 8 (the start
//     address), while insn is below end and the requested address is not below pc:
//     insn = execute_cfa_insn(insn, work, &info, &pc). (The first test is made before the loop.)
//  7. The first 1144 bytes of the work record are copied to the caller's record, which is returned.
// Inferred: the descriptor is a frame description entry and the routine is the DWARF-2 call-frame
// interpreter driver of the GNU runtime library; record fields are named only by offset.
// Unknown: the layout of the work record and of info beyond the offsets above; why the work record is
// cleared for 1152 bytes while 1144 are copied out (the extra 8 bytes are not copied).
// Not authored here: find_fde, extract_cie_info, execute_cfa_insn, decode_uleb128 (local routines of the
// original, kept as imports under their symbol-table names).
extern "C" {
struct NorrathCieInfo {
    const char* augmentation;
    unsigned word4;
    unsigned word8;
    unsigned wordC;
    unsigned short ra_column;
    unsigned short pad16;
    unsigned tail[3];
};
struct NorrathFrameState {
    unsigned char bytes[1144];
};
struct NorrathFde {
    unsigned length;
    unsigned cie_delta;
    unsigned pc_begin;
    unsigned pc_range;
};

void* memset(void* destination, int value, unsigned long size);
NorrathFde* find_fde(void* address);
unsigned char* extract_cie_info(NorrathFde* fde, NorrathCieInfo* info);
unsigned char* execute_cfa_insn(unsigned char* insn, void* state, NorrathCieInfo* info, unsigned char** pc);
unsigned char* decode_uleb128(unsigned char* insn, int* value);

NorrathFrameState* __frame_state_for(void* address, NorrathFrameState* out) {
    NorrathCieInfo info;
    alignas(8) unsigned char work[1152];
    int length;
    unsigned char* pc;
    NorrathFde* fde = find_fde(address);
    if (fde == nullptr) return nullptr;
    unsigned char* insn = extract_cie_info(fde, &info);
    if (insn == nullptr) return nullptr;
    memset(work, 0, 1152);
    const unsigned char* cie = reinterpret_cast<unsigned char*>(&fde->cie_delta) - fde->cie_delta;
    *reinterpret_cast<unsigned*>(work + 4) = info.word4;
    *reinterpret_cast<unsigned short*>(work + 1018) = info.ra_column;
    const unsigned char* cie_end = cie + *reinterpret_cast<const unsigned*>(cie) + 4;
    while (insn < cie_end) insn = execute_cfa_insn(insn, work, &info, nullptr);
    insn = reinterpret_cast<unsigned char*>(fde) + 16;
    if (info.augmentation[0] == 'z') {
        insn = decode_uleb128(insn, &length);
        insn += length;
    }
    const unsigned char* end = reinterpret_cast<unsigned char*>(fde) + fde->length + 4;
    pc = reinterpret_cast<unsigned char*>(fde->pc_begin);
    if (insn < end && !(address < pc)) {
        do {
            insn = execute_cfa_insn(insn, work, &info, &pc);
        } while (insn < end && !(address < pc));
    }
    for (int i = 0; i < 1144; ++i) out->bytes[i] = work[i];
    return out;
}
}

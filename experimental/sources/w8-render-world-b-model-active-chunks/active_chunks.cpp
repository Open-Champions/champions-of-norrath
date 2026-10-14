// Observed modelGetActiveChunks__FP12_modelHeaderi at 0x0016c970, 88 bytes.
// Returns a 64-bit mask. It starts as the bitwise complement of the 64-bit
// word at header+8. When the half-word at header+16 has bit 1 set and the
// second argument is nonzero, a table of 16-byte records at
// header + (byte at header+33) * 16 is walked, one record per low-to-high bit
// of the second argument, stopping after its highest set bit. For every set
// bit the record's first 64-bit word is OR'd into the result and the
// complement of its second 64-bit word is AND'd in.
// The meaning of the mask and records is not established here.
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(const void* model, unsigned selected) {
    const unsigned char* header = static_cast<const unsigned char*>(model);
    const unsigned short flags = *reinterpret_cast<const unsigned short*>(header + 16);
    unsigned long long result = ~*reinterpret_cast<const unsigned long long*>(header + 8);
    if ((flags & 2u) != 0 && selected != 0) {
        const unsigned char* record = header + (static_cast<unsigned>(header[33]) << 4);
        do {
            if ((selected & 1u) != 0) {
                const unsigned long long add = *reinterpret_cast<const unsigned long long*>(record);
                const unsigned long long remove = *reinterpret_cast<const unsigned long long*>(record + 8);
                result = (result | add) & ~remove;
            }
            selected >>= 1;
            record += 16;
        } while (selected != 0);
    }
    return result;
}

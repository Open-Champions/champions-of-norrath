/* soundFindGlobalDialog__FPc at 0x0024a1f0 is 212 bytes. The same search as
 * soundFindDialog__FPc (sources/w8-audio-calls-find-dialog), over the table
 * returned by ResourceMap::get on the global dialog map (globalDialogDir,
 * 0x007d8a90, the 12-byte map set up by the unit's static initializer)
 * instead of the per-level directory.
 *
 * The table is made of 80-byte records ended by a record whose first byte
 * is zero. For records whose name equals the argument (strcmp), the last
 * one whose signed halfword at offset 74 equals the signed byte `language`
 * (0x004844b4) is preferred; failing that, the last one whose halfword is
 * zero. With neither, or with no table, the result is null. A table whose
 * first byte is zero is not searched.
 */
extern "C" {
extern signed char language;
extern unsigned char norrath_sound_global_dialog_map_view[];
unsigned char* get__11ResourceMap(void* map);
int strcmp(const char* left, const char* right);

unsigned char* soundFindGlobalDialog__FPc(char* name) {
    unsigned char* table = get__11ResourceMap(norrath_sound_global_dialog_map_view);
    if (table == nullptr) return nullptr;
    int preferred = -1;
    int fallback = -1;
    if (table[0] != 0) {
        const unsigned char* record = table;
        int index = 0;
        do {
            if (strcmp(name, reinterpret_cast<const char*>(record)) == 0) {
                const short tag = static_cast<short>(record[74] | (record[75] << 8));
                if (tag == 0) fallback = index;
                if (tag == language) preferred = index;
            }
            record += 80;
            ++index;
        } while (record[0] != 0);
    }
    if (preferred < 0) {
        if (fallback < 0) return nullptr;
        preferred = fallback;
    }
    return table + preferred * 80;
}
}

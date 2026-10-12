/* soundFindDialog__FPc at 0x0024a120 is 172 bytes. soundGetDialogDir__Fv
 * returns a table of 80-byte records ended by a record whose first byte is
 * zero. The name field starts at offset 0; a signed halfword at offset 74 is
 * compared with the signed byte `language` (0x004844b4). The function
 * returns a pointer to a matching record, or null.
 *
 * Every record whose name equals the argument (strcmp) is considered. The
 * last such record whose halfword equals `language` is preferred. If there
 * is none, the last such record whose halfword is zero is used. With
 * neither, or with no table, the result is null. A table whose first byte
 * is zero is not searched.
 *
 * What the other record fields hold is outside this function; offsets 64,
 * 68, 72 and 76 are used by the stream start callers.
 */
extern "C" {
extern signed char language;
unsigned char* soundGetDialogDir__Fv();
int strcmp(const char* left, const char* right);

unsigned char* soundFindDialog__FPc(char* name) {
    unsigned char* table = soundGetDialogDir__Fv();
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

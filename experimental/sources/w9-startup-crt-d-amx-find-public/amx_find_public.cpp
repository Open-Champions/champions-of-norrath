// Observed amx_FindPublic at 0x00135bb8, 268 bytes. Arguments: the abstract
// machine object, a name, and a result slot. The first word of the machine
// object points at a header. In that header, the signed halfword at +32 is the
// number of public records and the word at +36 is the byte offset of the first
// record from the header start. A record is 24 bytes: a word, then a name that
// starts at +4. The search is a binary search over those records, ordered by
// strcmp of the record name against the wanted name:
//   - the middle index is (low + high) / 2 rounding toward zero;
//   - the record name is copied with strcpy into a 32-byte local buffer, then
//     compared with strcmp(buffer, wanted); a record name above the wanted one
//     moves high to middle - 1, below moves low to middle + 1;
//   - an equal name stores the middle index through the result slot and returns 0.
// The loop ends when high < low, which stores 0x7fffffff through the slot and
// returns 19. The header pointer and the record count are re-read on every
// iteration, and the copy is skipped when the middle index is not below the
// count (the buffer then keeps its previous contents).
// Unknown: whether 19 is a named status, and the full machine and header layout.
extern "C" {
char* strcpy(char* destination, const char* source);
int strcmp(const char* left, const char* right);

struct AmxView {
    const unsigned char* header;
};

int amx_FindPublic(AmxView* amx, const char* name, int* index) {
    char buffer[32];
    int low = 0;
    int high = *reinterpret_cast<const short*>(amx->header + 32) - 1;
    while (high >= low) {
        const int middle = (low + high) / 2;
        const unsigned char* const header = amx->header;
        if (middle < *reinterpret_cast<const short*>(header + 32)) {
            const unsigned offset = *reinterpret_cast<const unsigned*>(header + 36);
            strcpy(buffer, reinterpret_cast<const char*>(header + offset + middle * 24 + 4));
        }
        const int order = strcmp(buffer, name);
        if (order > 0) {
            high = middle - 1;
        } else if (order < 0) {
            low = middle + 1;
        } else {
            *index = middle;
            return 0;
        }
    }
    *index = 0x7fffffff;
    return 19;
}
}

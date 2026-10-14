// Observed: SetDefaultEntries__9VICSprite at 0x010b1838, 1576 bytes. The object holds 66 entries of 52 bytes (13 words) starting at this+424 (entry
// i is at this+424+52*i). It visits i = 0..65 in order and acts only when the first word of entry i is -1; what it does is chosen by a 66-way
// jump table at 0x011d42b0 that has six distinct targets (kinds below), and returns zero. Copying means all 52 bytes.
//   kind A (i = 0..6, 8..18, 34, 37, 38, 39, 45, 46): copy entry 0 into entry i.
//   kind B (i = 7, 56, 57, 59, 60, 61, 63, 64): copy entry 0, then store 1 into word 3 of entry i.
//   kind C (i = 23, 25, 26, 28, 29, 31, 33, 35, 41..44, 50..53): copy entry 23 into entry i when its first word is not -1, else entry 0.
//   kind D (i = 22, 24, 27, 30, 32, 36, 40): copy entry 22 when its first word is not -1, else entry 23 when that is not -1, else entry 0.
//   kind skip (i = 19, 20, 21, 47, 48, 49, 54, 55, 58, 62): nothing.
//   i = 65: if the word at this+408 is 9, copy entry 0, store 1 into word 3 of entry 65, and -1 into words 6 and 7. Otherwise, when entry 41's
//   first word is not -1, copy entry 41 into entry 65 and multiply its float word 2 by 0.5, then store -1 into words 6 and 7; when entry
//   41's first word is -1, copy entry 0, store 1 into word 3 and -1 into words 6 and 7.
// Because entries are filled in index order, a later entry sees the entries filled earlier in the same call.
// Inferred: entries are per-animation records where -1 in word 0 means "unset" and the missing ones inherit a related entry's contents (the
// fallback chain 23 / 22 / 0). The word at +408 is the race argument that Init__9VICSprite stored (value 9 is not known by name).
// Unknown: the meaning of the entry fields (word 2 is a float scale here, word 3 a flag-like word, words 6 and 7 reset to -1), and what the 66 ids are.
extern "C" {
static void copyEntry(unsigned char* bytes, int to, int from) {
    unsigned* d = reinterpret_cast<unsigned*>(bytes + 424 + 52 * to);
    const unsigned* s = reinterpret_cast<const unsigned*>(bytes + 424 + 52 * from);
    for (int k = 0; k < 13; ++k) d[k] = s[k];
}

static int firstWord(unsigned char* bytes, int entry) { return *reinterpret_cast<int*>(bytes + 424 + 52 * entry); }

int SetDefaultEntries__9VICSprite(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    static const char kKind[67] = "AAAAAAABAAAAAAAAAAASSSDCDCCDCCDCDCACDAAADCCCCAASSSCCCCSSBBSBBBSBBF";
    for (int i = 0; i < 66; ++i) {
        if (firstWord(bytes, i) != -1) continue;
        int* entry = reinterpret_cast<int*>(bytes + 424 + 52 * i);
        switch (kKind[i]) {
        case 'A':
            copyEntry(bytes, i, 0);
            break;
        case 'B':
            copyEntry(bytes, i, 0);
            entry[3] = 1;
            break;
        case 'C':
            copyEntry(bytes, i, firstWord(bytes, 23) != -1 ? 23 : 0);
            break;
        case 'D':
            copyEntry(bytes, i, firstWord(bytes, 22) != -1 ? 22 : firstWord(bytes, 23) != -1 ? 23 : 0);
            break;
        case 'F':
            if (*reinterpret_cast<int*>(bytes + 408) == 9) {
                copyEntry(bytes, i, 0);
                entry[3] = 1;
            } else if (firstWord(bytes, 41) != -1) {
                copyEntry(bytes, i, 41);
                *reinterpret_cast<float*>(entry + 2) = *reinterpret_cast<float*>(entry + 2) * 0.5f;
            } else {
                copyEntry(bytes, i, 0);
                entry[3] = 1;
            }
            entry[6] = -1;
            entry[7] = -1;
            break;
        default:
            break;
        }
    }
    return 0;
}
}

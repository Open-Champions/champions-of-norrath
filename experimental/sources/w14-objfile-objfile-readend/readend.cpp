// Observed: ReadEnd__9VIObjFile at 0x010a5ab0, 196 bytes. Offsets are bytes from the object file; D is the int at +232 and E(k)
// means the 32-byte record at this + 256 + 32 * k, of which words +8 and +20 (relative to the record) are used.
//  1. If the word at +576 is zero, return -1 with nothing changed.
//  2. When E(D).+8 is positive, Seek__6VIFilelQ26VIFile10SeekOrigin(this, E(D).+8, 0) is called and then E(D).+8 is set to zero
//     (D is read again afterwards).
//  3. +232 is set to D - 1 (call it N). If N is negative, +576 is set to zero. Otherwise E(N).+8 is reduced by E(N).+20 and then by 12.
//  4. In both cases +584 is set to zero and the result is 0.
// Inferred: a stack of 32-byte read-scope records indexed by +232; the record's +8 word counts bytes left in the scope, which the
// seek skips when unread data remains, and the parent scope is charged for the child's length plus a 12-byte header.
// The seek origin 0 is passed on as observed; the read flag is the word at +576.
// Unknown: member names, the record's other fields, and what origin 0 denotes.
extern "C" {
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long long offset, int origin);

int ReadEnd__9VIObjFile(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int* depth = reinterpret_cast<int*>(bytes + 232);
    if (*reinterpret_cast<int*>(bytes + 576) == 0) return -1;
    int* remaining = reinterpret_cast<int*>(bytes + 256 + 32 * *depth + 8);
    if (*remaining > 0) {
        Seek__6VIFilelQ26VIFile10SeekOrigin(object, *remaining, 0);
        *reinterpret_cast<int*>(bytes + 256 + 32 * *depth + 8) = 0;
    }
    int next = *depth - 1;
    *depth = next;
    if (next < 0) {
        *reinterpret_cast<int*>(bytes + 576) = 0;
    } else {
        *reinterpret_cast<int*>(bytes + 256 + 32 * next + 8) -= *reinterpret_cast<int*>(bytes + 256 + 32 * next + 20);
        *reinterpret_cast<int*>(bytes + 256 + 32 * *depth + 8) -= 12;
    }
    *reinterpret_cast<int*>(bytes + 584) = 0;
    return 0;
}
}

// Observed: Close__9VIObjFile at 0x010a64a8, 164 bytes. With the object file as this:
// if the word at +560 is zero it returns -1 and changes nothing. Otherwise, if the
// word at +572 is non-zero and the word at +232 is not -1, it calls WriteEnd__9VIObjFile(this)
// repeatedly until the word at +232 reads -1 (the word is re-read after every call).
// It then calls Flush__9VIObjFile(this) and Close__6VIFile(this) (the base file
// close, called on the same address), zeroes the eight words at +560 through +588,
// stores -1 at +232 and returns zero.
// Inferred: +560 is an "open" flag; +572 marks a write mode with an unfinished
// write stream at +232. Unknown: member names and the return values of the helpers.
extern "C" {
int WriteEnd__9VIObjFile(void* object);
int Flush__9VIObjFile(void* object);
int Close__6VIFile(void* object);

int Close__9VIObjFile(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int* state = reinterpret_cast<int*>(bytes + 232);
    int* words = reinterpret_cast<int*>(bytes + 560);
    if (words[0] == 0) return -1;
    if (words[3] != 0 && *state != -1) {
        do {
            WriteEnd__9VIObjFile(object);
        } while (*state != -1);
    }
    Flush__9VIObjFile(object);
    Close__6VIFile(object);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
    words[4] = 0;
    words[5] = 0;
    words[6] = 0;
    words[7] = 0;
    *state = -1;
    return 0;
}
}

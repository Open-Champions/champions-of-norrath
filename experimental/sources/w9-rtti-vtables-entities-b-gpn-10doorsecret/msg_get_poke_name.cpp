// Observed msg_getPokeName__10DoorSecretPUsi at 0x21bfc8, 60 bytes. The int is not read.
// Copies a one-character narrow string, a single space (at 0x485390 in the image's
// string area; this source holds its own copy), into the 16-bit string buffer with
// ustrcpy. Returns 1 when the string at levelName+81 equals "cellar1" (strcmp is 0),
// otherwise 0. The buffer is written in both cases.
// Unknown: why the result depends on the level name, and how large the buffer is.

extern "C" {
void ustrcpy__FPUsPc(unsigned short* destination, const char* source);
int strcmp(const char* a, const char* b);
extern char levelName[];
}

extern "C" int msg_getPokeName__10DoorSecretPUsi(void* self, unsigned short* name, int index) {
    (void)self;
    (void)index;
    ustrcpy__FPUsPc(name, " ");
    return strcmp(levelName + 81, "cellar1") == 0;
}

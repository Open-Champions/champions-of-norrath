// Observed msg_getPokeName__13AutoContainerPUsi at 0x225048, 72 bytes. The int is not
// read. When bit 2 (value 4) of the word at object+292 is set, copies the text-table
// entry for a short key into the 16-bit string buffer (textGetString on the key, then
// ustrcpy into the buffer). The key text sits at 0x4855d8 in the image's shared string
// area; this source holds its own copy. Always returns 1.
// Unknown: what the key selects at run time and how large the destination buffer is.

extern "C" {
unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
}

typedef unsigned Word __attribute__((may_alias));

extern "C" int msg_getPokeName__13AutoContainerPUsi(void* self, unsigned short* name, int index) {
    (void)index;
    const unsigned char* object = static_cast<const unsigned char*>(self);
    if ((*reinterpret_cast<const Word*>(object + 292) & 4) != 0)
        ustrcpy__FPUsT0(name, textGetString__FPc("Ignite"));
    return 1;
}

// Observed msg_getPokeName__9DoorSwingPUsi at 0x21bef0, 96 bytes. The int is not read.
// If the integer tag "disable" (findTagInt on the object, index 0) is nonzero, returns 0
// and leaves the buffer alone. Otherwise copies the text-table entry for the key
// "OpenDoor" into the 16-bit string buffer (textGetString, then ustrcpy) and returns 1.
// The tag name sits at 0x485360 and the key at 0x470590 in the image's string areas;
// this source holds its own copies.
// Unknown: how large the destination buffer is.

extern "C" {
int findTagInt__10GameObjectPci(void* object, const char* tag, int index);
unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
}

extern "C" int msg_getPokeName__9DoorSwingPUsi(void* self, unsigned short* name, int index) {
    (void)index;
    if (findTagInt__10GameObjectPci(self, "disable", 0) != 0) return 0;
    ustrcpy__FPUsT0(name, textGetString__FPc("OpenDoor"));
    return 1;
}

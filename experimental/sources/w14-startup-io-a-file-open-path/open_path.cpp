// Observed: Open__6VIFilePCci at 0x010c9e60 is 356 bytes and returns an int (0 or -1).
// Arguments: the file object, a name, and a mode whose low three bits are tested.
// The mode is stored at +4 first. A mode with none of bits 1, 2 and 4 returns -1
// at once. Otherwise: snprintf__8VIStringPciPCce(path, 256, "%s\\%s;1", "", name)
// into a 256-byte stack buffer; strupr__8VIStringPc(path); every '/' in it becomes
// a backslash; CDDiskReady__6VIFilei(this, 0); CDSearchFile__6VIFileP10sceCdlFILEPCc(
// this, this + 72, path). A zero from the search prints "VIEEFile: Unable to locate %s\n"
// with the path and returns -1. Otherwise, if _6VIFile$sIopStream is zero, the bytes
// at +108, +109 and +110 become 20, 1 and 0. "VIEEFile: %s opened\n" is printed with
// the path, the word at +0 and the 8 bytes at +16 are zeroed, BufferClear__6VIFile is
// called, and then: with bit 0x40 of the mode set, the word at +8 is zeroed and 0 is
// returned. Without it a 0x8010-byte block comes from __builtin_vec_new, its pointer
// goes to +184 even when it is null (then -1 is returned); otherwise +188 gets
// (pointer + 16) & -16, +8 is zeroed and 0 is returned.
// Inferred: this is the file-open entry for the disc path (mode bits 1, 2, 4 read,
// write, other); the 0x8010 block with a 16-byte aligned window is the buffer
// that BufferClear and the read paths use; the string literal "" is the first
// "%s" argument, an empty prefix. The class named in the messages is VIEEFile.
// Unknown: the meaning of the mode bits beyond the three tested, of the bytes at
// +108 to +110, and of the value that CDSearchFile writes at +72.
extern "C" {
extern int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...);
char* strupr__8VIStringPc(char* text);
void CDDiskReady__6VIFilei(void* file, int mode);
int CDSearchFile__6VIFileP10sceCdlFILEPCc(void* file, void* entry, const char* path);
int printf(const char* format, ...);
void BufferClear__6VIFile(void* file);
void* __builtin_vec_new(unsigned size);

int Open__6VIFilePCci(unsigned char* self, const char* name, int mode) {
    typedef unsigned long long __attribute__((may_alias)) Quad;
    char path[256];
    *reinterpret_cast<int*>(self + 4) = mode;
    if ((mode & 1) == 0 && (mode & 2) == 0 && (mode & 4) == 0) return -1;
    snprintf__8VIStringPciPCce(path, 256, "%s\\%s;1", "", name);
    strupr__8VIStringPc(path);
    for (int index = 0; path[index] != 0; ++index) {
        if (path[index] == '/') path[index] = '\\';
    }
    CDDiskReady__6VIFilei(self, 0);
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(self, self + 72, path) == 0) {
        printf("VIEEFile: Unable to locate %s\n", path);
        return -1;
    }
    if (norrath_file_iop_stream == 0) {
        self[108] = 20;
        self[109] = 1;
        self[110] = 0;
    }
    printf("VIEEFile: %s opened\n", path);
    *reinterpret_cast<int*>(self) = 0;
    *reinterpret_cast<Quad*>(self + 16) = 0;
    BufferClear__6VIFile(self);
    if ((*reinterpret_cast<int*>(self + 4) & 0x40) == 0) {
        void* block = __builtin_vec_new(0x8010);
        unsigned address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
        *reinterpret_cast<unsigned*>(self + 184) = address;
        if (block == 0) return -1;
        *reinterpret_cast<unsigned*>(self + 188) = (address + 16) & 0xfffffff0u;
    }
    *reinterpret_cast<int*>(self + 8) = 0;
    return 0;
}
}

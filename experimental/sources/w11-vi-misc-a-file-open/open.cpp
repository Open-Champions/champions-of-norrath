// Observed: Open__6VIFilePCci at 0x010c9e60 is 356 bytes. Arguments after the
// object: a path and an int mode.
//  - Word 4 of the object receives the mode. If none of bits 0, 1 and 2 is set it
//    returns -1.
//  - A 256-byte stack buffer is filled by the static
//    snprintf__8VIStringPciPCce(buf, 256, "%s\\%s;1", "", path), upper-cased with
//    strupr__8VIStringPc, and every '/' is replaced by '\\'.
//  - CDDiskReady__6VIFilei(this, 0) is called, then
//    CDSearchFile__6VIFileP10sceCdlFILEPCc(this, this + 72, buf). A zero result
//    prints "VIEEFile: Unable to locate %s\n" with the buffer and returns -1.
//  - Otherwise, if the word _6VIFile$sIopStream is zero, bytes 108, 109 and 110 of
//    the object become 20, 1 and 0. It prints "VIEEFile: %s opened\n" (buffer),
//    sets word 0 to 0 and the 64-bit value at 16 to 0, and calls
//    BufferClear__6VIFile(this).
//  - If bit 6 (0x40) of the mode is set, word 8 = 0 and it returns 0. Otherwise it
//    allocates 0x8010 bytes with __builtin_vec_new, stores the pointer in word 184
//    (before testing it), returns -1 if it is null, else stores the pointer rounded
//    up to 16 (p + 16 & -16) in word 188, sets word 8 = 0 and returns 0.
// Inferred: bits 0..2 are read/write/append-style open flags and 0x40 means the
// caller supplies its own buffer. Unknown: what the bytes at 108..110 mean.
extern "C" {
extern int norrath_vi_file_iop_stream __asm__("_6VIFile$sIopStream");

int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...);
char* strupr__8VIStringPc(char* text);
void CDDiskReady__6VIFilei(void* file, int mode);
int CDSearchFile__6VIFileP10sceCdlFILEPCc(void* file, void* record, const char* path);
int printf(const char* format, ...);
void BufferClear__6VIFile(void* file);
void* __builtin_vec_new(unsigned size);

int Open__6VIFilePCci(void* file, const char* path, int mode) {
    char* bytes = static_cast<char*>(file);
    char buffer[256];
    *reinterpret_cast<int*>(bytes + 4) = mode;
    if ((mode & 7) == 0) return -1;
    snprintf__8VIStringPciPCce(buffer, 256, "%s\\%s;1", "", path);
    strupr__8VIStringPc(buffer);
    if (buffer[0] != 0) {
        int index = 0;
        do {
            if (buffer[index] == '/') buffer[index] = '\\';
            ++index;
        } while (buffer[index] != 0);
    }
    CDDiskReady__6VIFilei(file, 0);
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(file, bytes + 72, buffer) == 0) {
        printf("VIEEFile: Unable to locate %s\n", buffer);
        return -1;
    }
    if (norrath_vi_file_iop_stream == 0) {
        bytes[108] = 20;
        bytes[109] = 1;
        bytes[110] = 0;
    }
    printf("VIEEFile: %s opened\n", buffer);
    *reinterpret_cast<int*>(bytes) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 16) = 0;
    BufferClear__6VIFile(file);
    if ((*reinterpret_cast<unsigned*>(bytes + 4) & 0x40) != 0) {
        *reinterpret_cast<int*>(bytes + 8) = 0;
        return 0;
    }
    char* block = static_cast<char*>(__builtin_vec_new(0x8010));
    *reinterpret_cast<char**>(bytes + 184) = block;
    if (block == nullptr) return -1;
    *reinterpret_cast<unsigned long*>(bytes + 188) = (reinterpret_cast<unsigned long>(block) + 16) & ~15ul;
    *reinterpret_cast<int*>(bytes + 8) = 0;
    return 0;
}
}

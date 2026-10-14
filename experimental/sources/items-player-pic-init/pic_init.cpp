// Observed picInit__4itemv at 0x1003c8, 224 bytes. It looks up piccache.hdr,
// allocates the size rounded up to a multiple of 2048, loads the file, and
// relocates the first two words of each 12-byte record by the buffer address.
// The record count is the first word. Records begin at buffer + 4.
extern "C" void cdDirectoryLookup__FPcPiT1i(char* name, int* first, int* second, int flag);
extern "C" void* memAlloc__FiPc(int bytes, char* label);
extern "C" void cdLoad__FPcPvii(char* name, void* dest, int bytes, int flag);

// Observed _4item$picLumpHeader at 0x48320c and _4item$nmPicGroups at 0x483210,
// 4 bytes each, both zero in the image. The record pointer is stored at _fbss
// (0x486800). _fbss is the entry BSS label and is not defined here.
extern "C" unsigned char* _4item$picLumpHeader = 0;
extern "C" int _4item$nmPicGroups = 0;
extern "C" unsigned char* _fbss;

static char piccache_name[] = "piccache.hdr";
static char piccache_label[] = "picCache header";

extern "C" void picInit__4itemv() {
    int size = 0;
    cdDirectoryLookup__FPcPiT1i(piccache_name, 0, &size, 0);
    unsigned aligned = (static_cast<unsigned>(size) + 2047u) & ~2047u;
    unsigned char* buffer = static_cast<unsigned char*>(memAlloc__FiPc(static_cast<int>(aligned), piccache_label));
    _4item$picLumpHeader = buffer;
    cdLoad__FPcPvii(piccache_name, buffer, static_cast<int>(aligned), 0);
    int count = *reinterpret_cast<int*>(buffer);
    unsigned char* records = buffer + 4;
    _fbss = records;
    _4item$nmPicGroups = count;
    if (count > 0) {
        for (int index = 0; index < count; ++index) {
            unsigned* words = reinterpret_cast<unsigned*>(records + index * 12);
            unsigned base = static_cast<unsigned>(reinterpret_cast<unsigned long>(buffer));
            words[0] += base;
            words[1] += base;
        }
    }
}

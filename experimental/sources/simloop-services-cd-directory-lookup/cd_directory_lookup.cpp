// Observed cdDirectoryLookup__FPcPiT1i at 0x00164f00, 540 bytes.
// The fourth argument is never read.
//
// sprintf writes name and the bytes "%s;1" from 0x00484000 into a stack
// buffer. Bytes a through z, the only _ctype_ entries at 0x0047e1f1 whose
// bit 2 is set, are reduced by 32. The result is compared with strcmp against
// 32-byte rows of currentLayerDir. The count word is at offset 24576.
//
// A hit stores the word at row+16 through the sector pointer, unless that
// pointer is null. When the directory pointer is layer1Dir (0x006d26a8),
// dualLayer2Start is added to that word. The word at row+20 is stored through
// the size pointer, unless that pointer is null. The result is 1.
//
// A miss while isDual is zero returns 0. A miss while isDual is nonzero and
// the current directory is layer0Dir (0x006cc6a0) calls cdWait, then
// sceCdLayerSearchFile with "\BG\DATA" and layer 1. When layer1Dir's count is
// zero and that search result is nonzero, loadDirectory receives layer1Dir,
// the word at the info buffer, and (the next word + 2047) logically shifted
// by 11. The scan then repeats on layer1Dir. Any other current directory
// waits and searches layer 0, without loadDirectory, then scans layer0Dir.
// A hit on that second scan stores the scanned directory in currentLayerDir
// and clears CDmode byte 1. A miss returns 0.
extern "C" {
extern unsigned char* currentLayerDir;
extern unsigned isDual;
extern unsigned dualLayer2Start;
extern unsigned char layer0Dir[];
extern unsigned char layer1Dir[];
extern unsigned char CDmode[4];

int sprintf(char* destination, const char* format, ...);
int strcmp(const char* left, const char* right);
void cdWait__Fv();
int sceCdLayerSearchFile(void* info, const char* name, int layer);
void loadDirectory__FP9directoryii(unsigned char* directory, int sector, int blocks);

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
static int row_count(const unsigned char* directory) {
    return static_cast<int>(load_word(directory + 24576));
}
static int find_row(const unsigned char* directory, const char* formatted) {
    const int count = row_count(directory);
    for (int index = 0; index < count; ++index) {
        if (strcmp(formatted, reinterpret_cast<const char*>(directory + index * 32)) == 0) return index;
    }
    return count;
}

int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* bytes, int) {
    char formatted[80];
    sprintf(formatted, "%s;1", name);
    for (char* cursor = formatted; *cursor != 0; ++cursor) {
        const unsigned char letter = static_cast<unsigned char>(*cursor);
        if (letter >= 'a' && letter <= 'z') *cursor = static_cast<char>(letter - 32u);
    }

    unsigned char* directory = currentLayerDir;
    int index = find_row(directory, formatted);
    if (isDual != 0u && index == row_count(directory)) {
        unsigned char info[64];
        unsigned char* scanned = layer0Dir;
        if (directory == layer0Dir) {
            cdWait__Fv();
            const int found = sceCdLayerSearchFile(info, "\\BG\\DATA", 1);
            scanned = layer1Dir;
            if (row_count(layer1Dir) == 0 && found != 0) {
                const unsigned size = load_word(info + 4);
                loadDirectory__FP9directoryii(layer1Dir, static_cast<int>(load_word(info)),
                                               static_cast<int>((size + 2047u) >> 11));
            }
        } else {
            cdWait__Fv();
            sceCdLayerSearchFile(info + 48, "\\BG\\DATA", 0);
        }
        index = find_row(scanned, formatted);
        if (index < row_count(scanned)) {
            currentLayerDir = scanned;
            CDmode[1] = 0;
            directory = scanned;
        } else {
            directory = currentLayerDir;
        }
    }
    if (index == row_count(directory)) return 0;
    if (sector != nullptr) {
        unsigned value = load_word(directory + index * 32 + 16);
        if (directory == layer1Dir) value += dualLayer2Start;
        *sector = static_cast<int>(value);
    }
    if (bytes != nullptr) *bytes = static_cast<int>(load_word(directory + index * 32 + 20));
    return 1;
}
}

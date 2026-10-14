// Observed addIcon__3mapP6Point3iPciT3N33 at 0x0015c320, 776 bytes. Arguments:
// point (address of a 12-byte Point3), kind (int), name (char*), type (int),
// then four more char* in the mangled name, here called offset_text, flag_text,
// extra_a and extra_b. The last two (pointers on the EE) and the lump result
// are only stored, never dereferenced, so they are treated as 32-bit words.
// Does nothing when name is null. Otherwise it first sets a float pair
// (dx, dy) to 0.0 and a word `flag` to 0. When offset_text is non-null it is
// parsed with sscanf using "%f,%f" into dx and dy; when flag_text is non-null
// it is parsed with sscanf using "%d" into flag. Then the resource name is
// built in a local 16-byte buffer: if the first four characters of name are
// not "flat", the buffer is sprintf("%s.tex", name). Otherwise a direction
// index k is taken from name: "flatup" 0, "flatrght" 1, "flatdown" 2,
// "flatleft" 3, any other name -1. With r = (k - type / 16384 + 4) % 4 (C
// division and remainder), the buffer becomes "flatup.tex" for r 0,
// "flatrght.tex" for 1, "flatdown.tex" for 2, "flatleft.tex" for 3; a negative
// r (only reachable when type / 16384 exceeds 3 more than k) leaves the buffer
// unwritten. Then, with i = _3map$iconListCount (0x004868b8) and the 28-byte
// record at _3map$iconList (0x006023c8) + 28 * i: +0 = point x + dx, +4 =
// point y + dy, +8 = lumpFindResource__FPcT0("mapicons.lmp", buffer), +12 =
// kind, +16 = 1.0 when flag is zero and otherwise the float literal 0.65
// (bits 0x3f266666, stored at 0x0047ff2c), +20 = extra_a, +24 = extra_b; and
// _3map$iconListCount becomes i + 1. The count is not checked against any
// capacity. Nothing is returned. What the fields and the flag mean is not
// established. Nothing here was executed on the EE.
extern "C" {
int sscanf(const char* text, const char* format, ...);
int sprintf(char* out, const char* format, ...);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, unsigned long count);
unsigned lumpFindResource__FPcT0(const char* lump, const char* name);
extern int _3map$iconListCount;
extern unsigned char _3map$iconList[];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;
typedef unsigned __attribute__((may_alias)) AliasedUnsigned;

extern "C" void addIcon__3mapP6Point3iPciT3N33(const float* point, int kind, char* name, int type,
                                               char* offset_text, char* flag_text, unsigned extra_a,
                                               unsigned extra_b) {
    float offset[2] = {0.0f, 0.0f};
    int flag = 0;
    if (name == nullptr) return;
    if (offset_text != nullptr) sscanf(offset_text, "%f,%f", &offset[0], &offset[1]);
    if (flag_text != nullptr) sscanf(flag_text, "%d", &flag);

    char buffer[16];
    if (strncmp(name, "flat", 4) != 0) {
        sprintf(buffer, "%s.tex", name);
    } else {
        int k = -1;
        if (strcmp(name, "flatup") == 0) k = 0;
        else if (strcmp(name, "flatrght") == 0) k = 1;
        else if (strcmp(name, "flatdown") == 0) k = 2;
        else if (strcmp(name, "flatleft") == 0) k = 3;
        const int r = (k - type / 16384 + 4) % 4;
        if (r == 1) __builtin_memcpy(buffer, "flatrght.tex", 13);
        else if (r == 0) __builtin_memcpy(buffer, "flatup.tex", 11);
        else if (r == 2) __builtin_memcpy(buffer, "flatdown.tex", 13);
        else if (r == 3) __builtin_memcpy(buffer, "flatleft.tex", 13);
    }

    const int i = _3map$iconListCount;
    unsigned char* record = _3map$iconList + 28 * i;
    *reinterpret_cast<AliasedFloat*>(record + 0) = point[0];
    *reinterpret_cast<AliasedFloat*>(record + 4) = point[1];
    *reinterpret_cast<AliasedFloat*>(record + 0) += offset[0];
    *reinterpret_cast<AliasedFloat*>(record + 4) += offset[1];
    const unsigned lump = lumpFindResource__FPcT0("mapicons.lmp", buffer);
    const int j = _3map$iconListCount;
    unsigned char* entry = _3map$iconList + 28 * j;
    *reinterpret_cast<AliasedUnsigned*>(entry + 8) = lump;
    *reinterpret_cast<AliasedInt*>(entry + 12) = kind;
    *reinterpret_cast<AliasedFloat*>(entry + 16) = flag != 0 ? 0.65f : 1.0f;
    _3map$iconListCount = j + 1;
    *reinterpret_cast<AliasedUnsigned*>(entry + 20) = extra_a;
    *reinterpret_cast<AliasedUnsigned*>(entry + 24) = extra_b;
}

// One function, two authored names: authored callers name it norrath_map_add_icon.
// The EE assembler and the host assembler both accept .set; Mach-O adds the
// leading underscore.
#if defined(__APPLE__)
asm(".globl _norrath_map_add_icon\n.set _norrath_map_add_icon, _addIcon__3mapP6Point3iPciT3N33\n");
#else
asm(".globl norrath_map_add_icon\n.set norrath_map_add_icon, addIcon__3mapP6Point3iPciT3N33\n");
#endif

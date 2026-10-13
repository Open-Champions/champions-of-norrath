#include "../w10-world-b-common/mem.hpp"
// Observed freeLump__FPv at 0x001646f8, 260 bytes in SLUS_205.65. Argument: a lump header
// (null is allowed and does nothing). The header's first halfword is an entry count (zero does
// nothing) and the entries follow at +4, 12 bytes each, with a name pointer in the first word
// and a data pointer in the second (the third word is not read). For each entry, in order,
// the text after the last '.' in the name (strrchr) is compared with strcmp against three
// literals: "tex" releases the data with textureFreeTexture__FP8_texture, "world" with
// worldFreeWorld__FP12_worldHeader, "uni" with textRemoveText__FPv. A name without a '.'
// and any other extension are skipped. Nothing is returned.
// Inferred: the three kinds are the texture, world and text resources a lump can hold.
// Unknown: the third entry word and the lump layout beyond what is read here.
extern "C" char* strrchr(const char* text, int character);
extern "C" int strcmp(const char* left, const char* right);
extern "C" void textureFreeTexture__FP8_texture(void* texture);
extern "C" void worldFreeWorld__FP12_worldHeader(void* world);
extern "C" void textRemoveText__FPv(void* text);

extern "C" void freeLump__FPv(void* lump) {
    using namespace norrath_wb;
    if (lump == 0) return;
    unsigned count = load_half(static_cast<unsigned char*>(lump));
    if (count == 0) return;
    unsigned char* entry = static_cast<unsigned char*>(lump) + 4;
    do {
        const char* name = reinterpret_cast<const char*>(load_pointer(entry));
        void* data = load_pointer(entry + 4);
        entry += 12;
        const char* dot = strrchr(name, '.');
        if (dot != 0) {
            const char* extension = dot + 1;
            if (strcmp(extension, "tex") == 0) textureFreeTexture__FP8_texture(data);
            else if (strcmp(extension, "world") == 0) worldFreeWorld__FP12_worldHeader(data);
            else if (strcmp(extension, "uni") == 0) textRemoveText__FPv(data);
        }
    } while (--count != 0);
}

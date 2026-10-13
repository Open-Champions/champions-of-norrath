// Observed clone__11LightEffect at 0x00126f70, 520 bytes: it takes a LightEffect and returns a new copy,
// or zero when the allocation fails. Offsets are decimal bytes.
// 1. blockAlloc__Fi (0x001441b8) is asked for 188 bytes; a null result is returned unchanged.
// 2. The word at +72 of the copy is first set to the GameObject vtable (_vt$10GameObject, 0x0044d498).
//    Three list heads at +0, +4 and +8 are copy-constructed from the source's with __8ListHeadRC8ListHead
//    (0x00194498). The word at +12 is copied. Words at +16 through +36 are copied, then the bytes at +40 and
//    +41, the words at +44, +48, +52, the halfword at +56, and the words at +60, +64, +68. Bytes +42, +43,
//    +58 and +59 are not written. The word at +72 is then set to the LightEffect vtable (_vt$11LightEffect,
//    0x00449a70).
// 3. The 60 bytes of the embedded light record from +76 to +135 are copied. If either of its first two
//    words in the copy is nonzero, both are cleared and lightAdd__FP5Light (0x0013d230) is called on
//    +76 of the copy.
// 4. The list head at +136 is copy-constructed from the source's, then the words at +140 through +176
//    (nine of them, in steps of four) and +180 and +184 are copied.
// The copy is returned. Inferred only: the first two words of the light record are chain links, so the
// copy re-registers itself. Unknown: the meaning of most copied fields.
extern "C" {
void* blockAlloc__Fi(int size);
void __8ListHeadRC8ListHead(void* destination, const void* source);
void lightAdd__FP5Light(void* light);
extern unsigned char game_object_vtable[] __asm__("_vt$10GameObject");
extern unsigned char light_effect_vtable[] __asm__("_vt$11LightEffect");
}

namespace {
inline unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
inline unsigned bits(const void* address) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(address)); }
}

extern "C" void* clone__11LightEffect(void* self) {
    unsigned char* source = static_cast<unsigned char*>(self);
    unsigned char* copy = static_cast<unsigned char*>(blockAlloc__Fi(188));
    if (copy == 0) return 0;
    word(copy, 72) = bits(game_object_vtable);
    __8ListHeadRC8ListHead(copy, source);
    __8ListHeadRC8ListHead(copy + 4, source + 4);
    __8ListHeadRC8ListHead(copy + 8, source + 8);
    word(copy, 12) = word(source, 12);
    for (unsigned offset = 16; offset < 40; offset += 4) word(copy, offset) = word(source, offset);
    copy[40] = source[40];
    copy[41] = source[41];
    for (unsigned offset = 44; offset < 56; offset += 4) word(copy, offset) = word(source, offset);
    *reinterpret_cast<unsigned short*>(copy + 56) = *reinterpret_cast<unsigned short*>(source + 56);
    for (unsigned offset = 60; offset < 72; offset += 4) word(copy, offset) = word(source, offset);
    word(copy, 72) = bits(light_effect_vtable);
    for (unsigned offset = 76; offset < 136; offset += 4) word(copy, offset) = word(source, offset);
    if (word(copy, 76) != 0 || word(copy, 80) != 0) {
        word(copy, 76) = 0;
        word(copy, 80) = 0;
        lightAdd__FP5Light(copy + 76);
    }
    __8ListHeadRC8ListHead(copy + 136, source + 136);
    for (unsigned offset = 140; offset < 188; offset += 4) word(copy, offset) = word(source, offset);
    return copy;
}

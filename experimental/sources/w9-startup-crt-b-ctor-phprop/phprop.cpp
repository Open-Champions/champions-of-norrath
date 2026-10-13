// Observed __6PhPropG6Point3PcN22liPQ24item11descriptionf at 0x00149068, 488
// bytes: the PhProp constructor. Arguments: the object, a position (copied to a
// local), three strings, a long, an integer (flags), a pointer to an item
// description record and a float. In the order of the image:
//  - Prop's constructor (0x001476b8) runs on the object with the position copy,
//    the three strings and the long;
//  - _vt$6PhProp (0x0044ce68) goes to object+72. Four halves at object+388,
//    +380, +372 and +364 become 0xffff; bytes +360 and +361 become 0xff; the
//    eight halves at +344..+358 become zero. object+12 gets bit 0x4000 set and
//    the float goes to object+576;
//  - when the description pointer is not null, 72 bytes from it replace
//    object+328..+399 (this covers the halves just written), byte +568 becomes
//    1, and object+12 gets 0x100000 set and 0x800 cleared;
//  - byte +570 and word +564 become zero, initStuff (0x00148738) runs with the
//    object and the flags;
//  - if byte +568 (as signed char) is nonzero and flags & 0x19 is nonzero, the
//    word at object+68 is stepped as x = x * 0x19660d + 0x3c6ef35f, the value
//    ((x >> 16) % 3) + 1 is formatted with sprintf using the string at
//    0x00462260 into a 16-byte-or-larger local buffer, lumpFindResource
//    (0x00165dc0) looks up that buffer in the string at 0x00483b70, and
//    soundPlaySequence (0x0024d308) plays the result on the object at 1.0f.
// Inferred: object+68 is a pseudo-random state (the step is the common linear
// congruence). Unknown: the meaning of the flag bits, of +568 and of the strings.
struct Point3 { float x, y, z; };

extern "C" {
void __4PropG6Point3PcN22li(void* self, Point3 position, char* first, char* second, char* third, long number);
void initStuff__6PhPropi(void* self, int flags);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
extern unsigned char _vt$6PhProp[];
extern char norrath_w9_startup_crt_b_view_462260[];
extern char norrath_w9_startup_crt_b_view_483b70[];

void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* first, char* second, char* third,
                                                    long number, int flags, const void* description, float scale) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __4PropG6Point3PcN22li(self, position, first, second, third, number);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$6PhProp));
    for (int i = 0; i < 4; ++i) *reinterpret_cast<unsigned short*>(object + 388 - 8 * i) = 0xffffu;
    object[361] = 0xff;
    object[360] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<unsigned short*>(object + 344 + 2 * i) = 0;
        *reinterpret_cast<unsigned short*>(object + 352 + 2 * i) = 0;
    }
    *reinterpret_cast<float*>(object + 576) = scale;
    unsigned flag_word = *reinterpret_cast<unsigned*>(object + 12) | 0x4000u;
    *reinterpret_cast<unsigned*>(object + 12) = flag_word;
    if (description) {
        __builtin_memcpy(object + 328, description, 72);
        object[568] = 1;
        *reinterpret_cast<unsigned*>(object + 12) = (*reinterpret_cast<unsigned*>(object + 12) | 0x100000u) & ~0x800u;
    }
    object[570] = 0;
    *reinterpret_cast<unsigned*>(object + 564) = 0;
    initStuff__6PhPropi(self, flags);
    if (static_cast<signed char>(object[568]) != 0 && (flags & 0x19) != 0) {
        char buffer[32];
        unsigned* seed = reinterpret_cast<unsigned*>(object + 68);
        *seed = *seed * 0x19660du + 0x3c6ef35fu;
        sprintf(buffer, norrath_w9_startup_crt_b_view_462260, static_cast<int>((*seed >> 16) % 3u) + 1);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(norrath_w9_startup_crt_b_view_483b70, buffer), self, 1.0f);
    }
}
}

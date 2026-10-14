// Observed __4PropG6Point3PcN22li at 0x001476b8, 504 bytes. Arguments: the object, a three-float
// position (by value), three text pointers (n1, n2, n3), a 64-bit value L and an int F. Steps:
//  1. The GameObject constructor runs on the object with the position and two null arguments.
//  2. The Prop table (_vt$4Prop) goes to object+72. n1 is copied to object+84, n2 to +100 and n3
//     to +164 with strcpy. The word at +76 is lumpFindResource__FPcT0(object+84, object+100) and
//     the word at +80 is lumpFindResource__FPcT0(object+84, object+164).
//  3. L is stored as 8 bytes at +232. The word at +68 is stepped as w*0x19660d + 0x3c6ef35f
//     (32-bit wrap) and the high 16 bits of the result go to the halfword at +56. The floats at
//     +48, +252 and +256 are cleared and the halfword at +268 is cleared.
//  4. modelGetBoundingBox__FP12_modelHeaderlPiN52 is called with (word +76, L, six int
//     addresses) filling b[0..5]. The floats at +240, +244 and +248 get the truncating-shift
//     centres (b[0]+b[3])>>1, (b[1]+b[4])>>1 and (b[2]+b[5])>>1, and the float at +264 gets the
//     third centre minus (float)b[2].
//  5. When bit 2 (value 4) of F is set, the three centres are added to the floats at +16, +20
//     and +24 (x, y, z order) and objectUpdateInGrid__FP10GameObject runs on the object.
//  6. init__4Propi is called with the object and F.
// Nothing here ran on the EE; the meaning of the fields is unknown.
struct Point3 { float x, y, z; };

extern "C" {
void game_object_construct(void* self, Point3 position, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
char* strcpy(char* to, const char* from);
void* lumpFindResource__FPcT0(char* lump, char* name);
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* header, long long l, int* a, int* b, int* c, int* d, int* e, int* f);
void objectUpdateInGrid__FP10GameObject(void* object);
void init__4Propi(void* object, int flags);
extern unsigned char vt_Prop[] __asm__("_vt$4Prop");
void construct(void* self, Point3 position, char* n1, char* n2, char* n3, long long value, int flags)
    __asm__("__4PropG6Point3PcN22li");
}

namespace {
unsigned low32(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
void* from_low32(unsigned w) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w)); }
}

extern "C" void construct(void* self, Point3 position, char* n1, char* n2, char* n3, long long value, int flags) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(o + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Prop));
    strcpy(reinterpret_cast<char*>(o + 84), n1);
    strcpy(reinterpret_cast<char*>(o + 100), n2);
    strcpy(reinterpret_cast<char*>(o + 164), n3);
    *reinterpret_cast<unsigned*>(o + 76) = low32(lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 84), reinterpret_cast<char*>(o + 100)));
    *reinterpret_cast<unsigned*>(o + 80) = low32(lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 84), reinterpret_cast<char*>(o + 164)));
    *reinterpret_cast<long long*>(o + 232) = value;
    unsigned w = *reinterpret_cast<unsigned*>(o + 68) * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<float*>(o + 48) = 0.0f;
    *reinterpret_cast<float*>(o + 256) = 0.0f;
    *reinterpret_cast<float*>(o + 252) = 0.0f;
    *reinterpret_cast<short*>(o + 268) = 0;
    *reinterpret_cast<unsigned*>(o + 68) = w;
    *reinterpret_cast<unsigned short*>(o + 56) = static_cast<unsigned short>(w >> 16);
    int b[6];
    modelGetBoundingBox__FP12_modelHeaderlPiN52(from_low32(*reinterpret_cast<unsigned*>(o + 76)), value, &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]);
    const float cz = static_cast<float>((b[2] + b[5]) >> 1);
    const float cx = static_cast<float>((b[0] + b[3]) >> 1);
    const float cy = static_cast<float>((b[1] + b[4]) >> 1);
    *reinterpret_cast<float*>(o + 248) = cz;
    *reinterpret_cast<float*>(o + 240) = cx;
    *reinterpret_cast<float*>(o + 244) = cy;
    *reinterpret_cast<float*>(o + 264) = cz - static_cast<float>(b[2]);
    if (flags & 4) {
        *reinterpret_cast<float*>(o + 16) += cx;
        *reinterpret_cast<float*>(o + 20) += cy;
        *reinterpret_cast<float*>(o + 24) += cz;
        objectUpdateInGrid__FP10GameObject(self);
    }
    init__4Propi(self, flags);
}

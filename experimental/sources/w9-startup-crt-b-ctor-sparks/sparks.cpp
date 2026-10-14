// Observed __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii at 0x0021d2a0, 432
// bytes: the Sparks constructor. Arguments: the object, two 12-byte values (the
// first is a position, the second is stored at object+88), a creature pointer,
// two sound-header pointers, and four integers (two in registers, two on the
// stack). The first sound-header pointer is not read. In the order of the image:
//  - Projectile's constructor (0x00153fd0) runs on the object with the position
//    copy, the creature and a null fourth argument;
//  - _vt$6Sparks (0x0044a270) goes to object+72. The light embedded at object+100
//    gets byte +9 = 0, 120.0f at +36 and +28, the float at 0x004816dc at +40
//    and +32, halves 318/193/-32 at +18/+20/+24 and again at +10/+12/+16, zero in
//    the word +44 and the halves +14 and +22, and byte +8 = 0 just before lightAdd
//    (0x0013d230) is called on it; the position is copied to object+148;
//  - the second 12-byte value is copied to object+88; object+176 and +180 get
//    the third and fourth integers (stack), frameNm (0x004844a4) + 600 goes to
//    object+84, the second integer to +168 and the first to +172, then
//    objectAddToRunList (0x00143830) runs on the object;
//  - lumpFindResource (0x00165dc0) is called with the strings at 0x00485380 and
//    0x004708d0, and soundPlaySequence (0x0024d308) with that result, the object
//    and 1.0f; finally the second sound-header pointer goes to object+160.
// Inferred: the integer-to-offset mapping is by position in the call; the offsets
// have no known names. Unknown: what the integers and the float at 0x004816dc mean.
struct Point3 { float x, y, z; };

extern "C" {
void __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(void* self, Point3 position, void* owner, void* map);
void lightAdd__FP5Light(void* light);
unsigned objectAddToRunList__FP10GameObject(void* object);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
extern unsigned char _vt$6Sparks[];
extern unsigned frameNm;
extern float norrath_w9_startup_crt_b_view_4816dc;
extern char norrath_w9_startup_crt_b_view_485380[];
extern char norrath_w9_startup_crt_b_view_4708d0[];

void __6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii(void* self, Point3 position, Point3 value, void* creature,
                                                      void* unused_header, void* header, int first, int second,
                                                      int third, int fourth) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* light = object + 100;
    __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(self, position, creature, nullptr);
    const Point3 kept = position;
    const float tint = norrath_w9_startup_crt_b_view_4816dc;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$6Sparks));
    light[9] = 0;
    __builtin_memcpy(object + 148, &kept, 12);
    *reinterpret_cast<float*>(light + 36) = 120.0f;
    *reinterpret_cast<float*>(light + 40) = tint;
    *reinterpret_cast<short*>(light + 18) = 318;
    *reinterpret_cast<short*>(light + 20) = 193;
    *reinterpret_cast<short*>(light + 24) = -32;
    *reinterpret_cast<float*>(light + 28) = 120.0f;
    *reinterpret_cast<float*>(light + 32) = tint;
    *reinterpret_cast<short*>(light + 10) = 318;
    *reinterpret_cast<short*>(light + 12) = 193;
    *reinterpret_cast<short*>(light + 16) = -32;
    *reinterpret_cast<int*>(light + 44) = 0;
    *reinterpret_cast<short*>(light + 14) = 0;
    *reinterpret_cast<short*>(light + 22) = 0;
    light[8] = 0;
    lightAdd__FP5Light(light);
    __builtin_memcpy(object + 88, &value, 12);
    *reinterpret_cast<int*>(object + 176) = third;
    *reinterpret_cast<int*>(object + 180) = fourth;
    *reinterpret_cast<unsigned*>(object + 84) = frameNm + 600u;
    *reinterpret_cast<int*>(object + 168) = second;
    *reinterpret_cast<int*>(object + 172) = first;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(norrath_w9_startup_crt_b_view_485380, norrath_w9_startup_crt_b_view_4708d0), self, 1.0f);
    *reinterpret_cast<unsigned*>(object + 160) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(header));
}
}

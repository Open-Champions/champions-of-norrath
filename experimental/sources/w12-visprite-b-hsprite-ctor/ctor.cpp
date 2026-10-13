// Observed __9VIHSprite at 0x0105f7d8, 532 bytes in the pinned SLUS-20565 image. With the object as
// this it does, in order:
//   1. __8VISprite(this); the address of _vt$9VIHSprite is stored at this+60.
//   2. Member constructors: __t8VIVector1Z13VIHSpriteNode(this+64), __t8VIVector1Z10VIMatrix44(this+76),
//      __t7VIArray1Z16VIHSpriteTrigger(this+88), __t6VIPool1Z19VIHSpriteAttachment(this+96),
//      __t6VIPool1Z13VIHSpritePlay(this+120), __t6VIPool1Z17VIHSpritePlayNode(this+144).
//   3. Stores, none of which overlap a member constructor's argument range:
//      word 0 at +168; word 4 at +52; float 1.0 at +208; word -1 over +184 up to +208 (six words);
//      float zero over +212 up to +236; two 32-byte records at +236 and +268, each holding floats
//      0, 0, 0, 1.0, 1.0, 0, 0, 0; word 0 at +300; zero over +304 up to +312; zero over +320 up to
//      +384 (64 bytes); words -1 at +384 and +388.
//   Nothing is returned. The original builds those ranges through temporaries and copies; only the
//   final contents are described here.
// Inferred: +64 is a vector of nodes, +76 a vector of 4x4 matrices, +88 an array of triggers and
// +96, +120, +144 pools (from the template names); each 32-byte record looks like a vector pair
// with a 1.0 pair; +320 is a zeroed 4x4 matrix; the vtable store is the class table.
// Unknown: the meaning of each field and the object size. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
void __t8VIVector1Z13VIHSpriteNode(void* self);
void __t8VIVector1Z10VIMatrix44(void* self);
void __t7VIArray1Z16VIHSpriteTrigger(void* self);
void __t6VIPool1Z19VIHSpriteAttachment(void* self);
void __t6VIPool1Z13VIHSpritePlay(void* self);
void __t6VIPool1Z17VIHSpritePlayNode(void* self);
extern unsigned char _vt$9VIHSprite[] __asm__("_vt$9VIHSprite");

void fn_9VIHSprite(void* self) __asm__("__9VIHSprite");
void fn_9VIHSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIHSprite));
    __t8VIVector1Z13VIHSpriteNode(bytes + 64);
    __t8VIVector1Z10VIMatrix44(bytes + 76);
    __t7VIArray1Z16VIHSpriteTrigger(bytes + 88);
    __t6VIPool1Z19VIHSpriteAttachment(bytes + 96);
    __t6VIPool1Z13VIHSpritePlay(bytes + 120);
    __t6VIPool1Z17VIHSpritePlayNode(bytes + 144);
    *reinterpret_cast<unsigned*>(bytes + 168) = 0;
    *reinterpret_cast<unsigned*>(bytes + 52) = 4;
    *reinterpret_cast<float*>(bytes + 208) = 1.0f;
    for (int i = 0; i < 6; ++i) *reinterpret_cast<unsigned*>(bytes + 184 + 4 * i) = static_cast<unsigned>(-1);
    float* z = reinterpret_cast<float*>(bytes + 212);
    for (int i = 0; i < 6; ++i) z[i] = 0.0f;
    for (int r = 0; r < 2; ++r) {
        float* rec = reinterpret_cast<float*>(bytes + 236 + 32 * r);
        rec[0] = 0.0f; rec[1] = 0.0f; rec[2] = 0.0f;
        rec[3] = 1.0f; rec[4] = 1.0f;
        rec[5] = 0.0f; rec[6] = 0.0f; rec[7] = 0.0f;
    }
    *reinterpret_cast<unsigned*>(bytes + 300) = 0;
    *reinterpret_cast<float*>(bytes + 304) = 0.0f;
    *reinterpret_cast<float*>(bytes + 308) = 0.0f;
    float* m = reinterpret_cast<float*>(bytes + 320);
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 384) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 388) = static_cast<unsigned>(-1);
}
}

// Observed: __13VIGroupSprite at 0x0101dab8 is 116 bytes in the pinned SLUS-20565 image. With the
// object as this it calls __8VISprite(this), stores the address of _vt$13VIGroupSprite at this+60,
// then for five records of 32 bytes starting at this+68 stores float zero in the three floats at
// record+4 and the three floats at record+20 (the words at record+0 and record+16 are not written),
// then stores 11 at this+52. Nothing is returned.
// Inferred: the word at this+60 receives the class table, as for the other VISprite subclasses; each
// record holds two vectors.
// Unknown: the meaning of the untouched words, of the value at this+52 and the object size; the class
// table is an import. Nothing here ran on the EE.
extern "C" {
void __8VISprite(void* self);
extern unsigned char _vt$13VIGroupSprite[] __asm__("_vt$13VIGroupSprite");

void fn_13VIGroupSprite(void* self) __asm__("__13VIGroupSprite");
void fn_13VIGroupSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VIGroupSprite));
    for (int i = 0; i < 5; ++i) {
        float* record = reinterpret_cast<float*>(bytes + 68 + 32 * i);
        record[1] = 0.0f; record[2] = 0.0f; record[3] = 0.0f;
        record[5] = 0.0f; record[6] = 0.0f; record[7] = 0.0f;
    }
    *reinterpret_cast<unsigned*>(bytes + 52) = 11;
}
}

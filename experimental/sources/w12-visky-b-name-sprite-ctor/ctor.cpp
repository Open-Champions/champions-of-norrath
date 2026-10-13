// Observed: __12VINameSprite at 0x010a1000, 80 bytes. With the object as this it calls
// __8VISprite(this); stores the address of _vt$12VINameSprite (0x01207d08) at this+60; stores
// 0.0f in the four floats at this+100, +104, +108 and +112; then calls Clear__12VINameSprite(this).
// Nothing is returned.
// Inferred: the float group at +100 is a vector that Clear overwrites afterwards (it stores 1.0f
// there); the +60 word is the virtual table of the VISprite base.
// Unknown: what the four floats at +100 represent.
extern "C" {
void __8VISprite(void* self);
int Clear__12VINameSprite(void* self);
extern unsigned char _vt$12VINameSprite[];

void __12VINameSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VINameSprite));
    float* vec = reinterpret_cast<float*>(bytes + 100);
    vec[3] = 0.0f;
    vec[0] = 0.0f;
    vec[1] = 0.0f;
    vec[2] = 0.0f;
    Clear__12VINameSprite(self);
}
}

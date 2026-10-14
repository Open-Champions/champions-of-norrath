// Observed: _$_8VISprite at 0x010080e8, 52 bytes, returns nothing. With (this, flags) it stores the address of _vt$8VISprite at
// this+60 and, when bit 0 of flags is set, tail-calls __builtin_delete(this). It calls no base destructor.
// Inferred: the destructor of VISprite; +60 is the table pointer slot the class uses, set back to the class's own table as a
// destructor does. flags bit 0 is the delete-after flag.
// Unknown: other members of the object and whether a base class has a destructor of its own.
extern "C" {
extern unsigned char vt_VISprite[] __asm__("_vt$8VISprite");
void __builtin_delete(void* pointer);

void _$_8VISprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VISprite));
    if ((flags & 1) != 0) __builtin_delete(self);
}
}

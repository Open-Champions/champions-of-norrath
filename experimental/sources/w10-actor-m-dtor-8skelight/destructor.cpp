// Observed _$_8Skelight at 0x3163a8, 100 bytes. Stores the low 32 bits of the address of _vt$8Skelight
// (0x44fdf8, the authored view norrath_ctors_lava_wraith_view_44fdf8) at object+72, then runs _$_8Creature
// on the object with flags 0, and when bit 0 of the incoming flags was set frees the object with
// blockFree__FPv. Other flag bits do nothing. Nothing is returned.
extern "C" {
void _$_8Creature(void* object, int flags);
void blockFree__FPv(void* block);
extern unsigned char norrath_ctors_lava_wraith_view_44fdf8[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void _$_8Skelight(unsigned char* object, int flags) {
    *reinterpret_cast<AliasedWord*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44fdf8));
    const int release = flags & 1;
    _$_8Creature(object, 0);
    if (release != 0) blockFree__FPv(object);
}

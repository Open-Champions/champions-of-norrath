// Observed _$_4Soul at 0x314278, 116 bytes. Stores the low 32 bits of the address of _vt$4Soul
// (0x44def0, the authored view norrath_ctors_lava_wraith_view_44def0) at object+72. Then it destroys
// the lists headed at object+964 and then object+960 with listDestroy__FP8ListHead, runs _$_8Creature
// on the object with flags 0, and when bit 0 of the incoming flags was set frees the object with
// blockFree__FPv. Other flag bits do nothing. Nothing is returned. Inferred: the two list heads are the
// minion lists the constructor links into.
extern "C" {
void listDestroy__FP8ListHead(void* head);
void _$_8Creature(void* object, int flags);
void blockFree__FPv(void* block);
extern unsigned char norrath_ctors_lava_wraith_view_44def0[];
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void _$_4Soul(unsigned char* object, int flags) {
    *reinterpret_cast<AliasedWord*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44def0));
    const int release = flags & 1;
    listDestroy__FP8ListHead(object + 964);
    listDestroy__FP8ListHead(object + 960);
    _$_8Creature(object, 0);
    if (release != 0) blockFree__FPv(object);
}

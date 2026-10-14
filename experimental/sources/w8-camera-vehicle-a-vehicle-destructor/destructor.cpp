// Observed _$_7Vehicle at 0x00270598, 212 bytes.
// Stores the low 32 bits of the address of _vt$7Vehicle (0x0044ffd8) at
// object+72. With i = the word at object+2156 and n = vehicleCount
// (0x00485e40), when i < n-1 the entries of vehicleList (0x00436fa8, pointer
// array) from index i are each replaced by the next entry, and for each such
// step the word at +2156 of the entry that moved down is decreased by one;
// that is n-1-i steps, in increasing index order. Then vehicleCount becomes n-1
// (n is read before the moves and again after them; the value is the same).
// The list at object+2136 is destroyed with listDestroy__FP8ListHead, the
// AnimationState destructor _$_14AnimationState runs on object+120 with flags 0,
// and the GameObject destructor _$_10GameObject runs with flags 0. Bit 0 of the
// incoming flag selects blockFree__FPv on the object last. The object's own
// slot in the list is not checked, and a stale index is not repaired. Nothing is
// returned. Nothing here was executed on the EE.
extern "C" {
void listDestroy__FP8ListHead(void* list);
void _$_14AnimationState(void* state, int flags);
void _$_10GameObject(void* object, int flags);
void blockFree__FPv(void* object);
extern unsigned char _vt$7Vehicle[];
extern unsigned char* vehicleList[];
extern int vehicleCount;
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void _$_7Vehicle(unsigned char* object, int flags) {
    *reinterpret_cast<AliasedWord*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$7Vehicle));
    const int release = flags & 1;
    const int last = vehicleCount - 1;
    const int index = *reinterpret_cast<AliasedInt*>(object + 2156);
    if (index < last) {
        for (int i = index; i < last; ++i) {
            unsigned char* next = vehicleList[i + 1];
            vehicleList[i] = next;
            *reinterpret_cast<AliasedInt*>(next + 2156) -= 1;
        }
    }
    vehicleCount = vehicleCount - 1;
    listDestroy__FP8ListHead(object + 2136);
    _$_14AnimationState(object + 120, 0);
    _$_10GameObject(object, 0);
    if (release != 0) blockFree__FPv(object);
}

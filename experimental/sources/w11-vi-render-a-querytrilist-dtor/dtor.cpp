// Observed: _$_14VIQueryTriList at 0x010d0fd0 is 120 bytes and returns nothing.
// It calls Clear__14VIQueryTriList(this), then _$_t7VIArray1Z7VIVect3 with flags 2
// on offset 20 and then on offset 12, then, if bit 0 of the second argument is
// set, tail-calls __builtin_delete(this).
extern "C" {
int Clear__14VIQueryTriList(void* object);
void _$_t7VIArray1Z7VIVect3(void* array, int flags);
void __builtin_delete(void* pointer);

void _$_14VIQueryTriList(void* object, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    Clear__14VIQueryTriList(object);
    _$_t7VIArray1Z7VIVect3(bytes + 20, 2);
    _$_t7VIArray1Z7VIVect3(bytes + 12, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}

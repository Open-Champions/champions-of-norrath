// Observed: _$_12VIAtmosphere at 0x0106b310 is 164 bytes and returns nothing. It
// calls _$_7VILList with flags 2 on offsets 4464, 4448, then _$_10VIMatrix44 with
// flags 2 on 1408, 1344, 1280, then _$_7VILList with flags 2 on 36 and 20, then,
// if bit 0 of the second argument is set, tail-calls __builtin_delete(this).
extern "C" {
void _$_7VILList(void* list, int flags);
void _$_10VIMatrix44(void* matrix, int flags);
void __builtin_delete(void* pointer);

void _$_12VIAtmosphere(void* object, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    _$_7VILList(bytes + 4464, 2);
    _$_7VILList(bytes + 4448, 2);
    _$_10VIMatrix44(bytes + 1408, 2);
    _$_10VIMatrix44(bytes + 1344, 2);
    _$_10VIMatrix44(bytes + 1280, 2);
    _$_7VILList(bytes + 36, 2);
    _$_7VILList(bytes + 20, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}

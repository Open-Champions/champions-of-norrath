// Observed: _$_7VILList at 0x010727a0 is 44 bytes and returns nothing. If bit 0
// of the second argument is set it tail-calls __builtin_delete(this); otherwise it
// does nothing. The list is not walked or cleared.
extern "C" {
void __builtin_delete(void* pointer);

void _$_7VILList(void* list, int flags) {
    if ((flags & 1) != 0) __builtin_delete(list);
}
}

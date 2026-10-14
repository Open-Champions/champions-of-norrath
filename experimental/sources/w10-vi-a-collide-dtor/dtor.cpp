// Observed: _$_9VICollide at 0x01096d80 is 124 bytes and returns nothing. In
// order it calls Close__9VICollide(this); the VIPool<VICollBuffer*> destructor
// (_$_t6VIPool1ZP12VICollBuffer) on this + 528 with 2; the VIMatrix44 destructor
// on this + 80 with 2 and then on this + 16 with 2. If bit 0 of the second
// argument is set it tail-calls __builtin_delete(this).
extern "C" {
int Close__9VICollide(void* object);
void _$_t6VIPool1ZP12VICollBuffer(void* pool, int flags);
void _$_10VIMatrix44(void* matrix, int flags);
void __builtin_delete(void* pointer);

void _$_9VICollide(void* object, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    Close__9VICollide(object);
    _$_t6VIPool1ZP12VICollBuffer(bytes + 528, 2);
    _$_10VIMatrix44(bytes + 80, 2);
    _$_10VIMatrix44(bytes + 16, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}

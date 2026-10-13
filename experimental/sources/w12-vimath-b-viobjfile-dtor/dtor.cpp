// Observed: _$_9VIObjFile at 0x010a60d8 is 108 bytes. With (this, flags): if the word at offset
// 560 is nonzero it calls Close__9VIObjFile(this); it then calls _$_6VIFile(this, 2); and when
// bit 0 of flags is set it tail calls __builtin_delete(this). Nothing is returned.
// Inferred: a destructor that closes an open object file, destroys the base without deleting
// it, then optionally frees the object. The meaning of the word at offset 560 is not proven.
// Unknown: the object size.
extern "C" {
void Close__9VIObjFile(void* self);
void _$_6VIFile(void* self, int flags);
void __builtin_delete(void* pointer);

void _$_9VIObjFile(void* self, int flags) {
    if (*reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 560) != 0) Close__9VIObjFile(self);
    _$_6VIFile(self, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}

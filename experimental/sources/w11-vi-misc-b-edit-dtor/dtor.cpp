// Observed: _$_9VIWndEdit at 0x010ad4c8 is 140 bytes. With (this, flags) it stores the address of
// _vt$9VIWndEdit (0x01208040) at this+32, then if the word at this+424 is nonzero calls
// __builtin_vec_delete(word), then if the word at this+432 is nonzero calls
// __builtin_vec_delete(word), then runs _$_5VIWnd(this, 0), and when bit 0 of flags is set tail-
// calls __builtin_delete(this). Nothing is returned.
// Inferred: the words at +424 and +432 are array blocks owned by the window (freed with the array
// delete); their purpose is not established here.
extern "C" {
void _$_5VIWnd(void* self, int flags);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);
extern unsigned char _vt$9VIWndEdit[];

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}

void dtor_9VIWndEdit(void* self, int flags) asm("_$_9VIWndEdit");
void dtor_9VIWndEdit(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 32) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEdit));
    if (unsigned block = *reinterpret_cast<unsigned*>(bytes + 424)) __builtin_vec_delete(pointer_of(block));
    if (unsigned block = *reinterpret_cast<unsigned*>(bytes + 432)) __builtin_vec_delete(pointer_of(block));
    _$_5VIWnd(self, 0);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}

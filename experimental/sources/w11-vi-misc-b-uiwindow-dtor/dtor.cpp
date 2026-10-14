// Observed: _$_10VIUIWindow at 0x0103ef78 is 44 bytes. With (this, flags) it does nothing except,
// when bit 0 of flags is set, tail-call __builtin_delete(this). It writes no table word.
// Unknown: whether the class has a table at all, and why the flags argument is passed as 2 by
// every embedding destructor.
extern "C" {
void __builtin_delete(void* pointer);

void dtor_10VIUIWindow(void* self, int flags) asm("_$_10VIUIWindow");
void dtor_10VIUIWindow(void* self, int flags) {
    if ((flags & 1) != 0) __builtin_delete(self);
}
}

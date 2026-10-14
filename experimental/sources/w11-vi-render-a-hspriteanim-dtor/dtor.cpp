// Observed: _$_13VIHSpriteAnim at 0x0105cef0 is 84 bytes and returns nothing. It
// calls Clear__13VIHSpriteAnim(this), then, if bit 0 of the second argument is
// set, tail-calls __builtin_delete(this).
extern "C" {
int Clear__13VIHSpriteAnim(void* object);
void __builtin_delete(void* pointer);

void _$_13VIHSpriteAnim(void* object, int flags) {
    Clear__13VIHSpriteAnim(object);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}

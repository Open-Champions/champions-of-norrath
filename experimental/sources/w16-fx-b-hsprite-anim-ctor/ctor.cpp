// Observed: __13VIHSpriteAnim at 0x0105cec8 is 36 bytes. It stores zero to this+52, this+4 and this+0 (in that order) and then
// tail-calls Clear__13VIHSpriteAnim(this), so the value left in the return register is whatever Clear returns (Clear ends by returning zero).
// Inferred: this is the default constructor; the first two words and the word at +52 are made null before Clear runs, which deletes the
// array at +52 when it is nonnull.
// Unknown: what the words at +0 and +4 are.
extern "C" {
int Clear__13VIHSpriteAnim(void* self);

void __13VIHSpriteAnim(char* self) {
    *(int*)(self + 52) = 0;
    *(int*)(self + 4) = 0;
    *(int*)(self + 0) = 0;
    Clear__13VIHSpriteAnim(self);
}
}

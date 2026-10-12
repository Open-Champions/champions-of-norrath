// Observed collisionFrame, a 4-byte .sdata object at 0x484188 whose stored word
// is zero in the image. The collision code reads one word from it. Zero words
// are BSS in this build, so the definition is zero-initialized storage.
// Unknown: every store made by code outside this translation unit.
extern "C" {
unsigned char norrath_collision_frame_bytes[4] = {};
}

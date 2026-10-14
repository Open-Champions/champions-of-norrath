// Observed: GetLightUISprite__7VIScene at 0x01144d10, 8 bytes. Returns the 32-bit word at scene offset 1132, nothing else is read or written.
// Inferred: an index of a sprite in the scene's sprite pool, kept so the scene can address a built-in interface sprite; -1 is the pool's
// usual empty marker.
// Unknown: when the word is set and the sprite's role beyond the accessor name.
extern "C" {
int GetLightUISprite__7VIScene(unsigned char* self) {
    return *reinterpret_cast<int*>(self + 1132);
}
}

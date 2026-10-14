// Observed: GetParticleUISprite__7VIScene at 0x01144d40, 8 bytes. It returns the signed int word at byte 1136 of its argument and writes nothing.
// Inferred: a sprite pointer or handle of the scene. Unknown: the member name.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
int GetParticleUISprite__7VIScene(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 1136);
}
}

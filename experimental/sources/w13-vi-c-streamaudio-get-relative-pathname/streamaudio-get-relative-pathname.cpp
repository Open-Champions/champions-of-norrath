// Observed: GetRelativePathname__C19VIStreamAudioSprite at 0x01019348, 8 bytes.
// It returns the address this + 72 and reads and writes nothing.
// Inferred: the bytes from offset 72 hold the sprite's relative path as an
// in-object character array (the result is the array address, not a loaded
// pointer). Unknown: the array length and the member name.
extern "C" {
const char* GetRelativePathname__C19VIStreamAudioSprite(const void* object) {
    return reinterpret_cast<const char*>(static_cast<const unsigned char*>(object) + 72);
}
}

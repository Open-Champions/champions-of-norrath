// Observed: SetTracks__17VISceneSoundTrack at 0x0114e9f0, 320 bytes. With the object as this it
// stores only constants and returns zero. Offsets are bytes from the object:
//   +136 = 0 and +140 = 0
//   +144 .. +188, twelve 32-bit words, and +192 .. +220, eight 32-bit words, with the values below.
// Inferred only from their spread: the values look like 32-bit hash words, not counts or flags.
// Unknown: what the twenty words identify, and what the two zeroed words at +136/+140 count.
extern "C" {
int SetTracks__17VISceneSoundTrack(void* self) {
    static const unsigned first[12] = {
        0x9a5dbaccu, 0xa5c60038u, 0xebf2eec3u, 0x909e78e7u, 0x92c9a8ceu, 0xa0dbe44au,
        0xb9422f46u, 0x2feb20f5u, 0x864a0682u, 0x656f64e9u, 0xdb5aa9fbu, 0x2143b5dfu};
    static const unsigned second[8] = {
        0x42e6cf3au, 0x72f08216u, 0x1adcc066u, 0x68b480a2u,
        0x9e562b5au, 0xcef71f36u, 0x0ce4ab35u, 0xfd6feee9u};
    unsigned* words = static_cast<unsigned*>(self);
    for (int i = 0; i < 8; ++i) words[48 + i] = second[i];
    for (int i = 0; i < 12; ++i) words[36 + i] = first[i];
    words[34] = 0;
    words[35] = 0;
    return 0;
}
}

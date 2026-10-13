#include "set_tracks.cpp"

int main() {
    alignas(8) static unsigned words[64];
    for (int i = 0; i < 64; ++i) words[i] = 0x5a5a5a5au;
    if (SetTracks__17VISceneSoundTrack(words) != 0) return 1;
    static const unsigned expect[22] = {
        0, 0,
        0x9a5dbaccu, 0xa5c60038u, 0xebf2eec3u, 0x909e78e7u, 0x92c9a8ceu, 0xa0dbe44au,
        0xb9422f46u, 0x2feb20f5u, 0x864a0682u, 0x656f64e9u, 0xdb5aa9fbu, 0x2143b5dfu,
        0x42e6cf3au, 0x72f08216u, 0x1adcc066u, 0x68b480a2u,
        0x9e562b5au, 0xcef71f36u, 0x0ce4ab35u, 0xfd6feee9u};
    for (int i = 0; i < 64; ++i) {
        unsigned want = (i >= 34 && i < 56) ? expect[i - 34] : 0x5a5a5a5au;
        if (words[i] != want) return 2 + i;
    }
    return 0;
}
